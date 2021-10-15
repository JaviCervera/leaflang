#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif
#include <numeric>
#include "../src/pico.h"
#include "../src/swan/strmanip.hh"

using namespace std;
using namespace swan::strmanip;

string GetFilename(int argc, char** argv) {
    if (argc >= 2) {
        return argv[argc-1];
    } else {
        Error("No input file specified.");
        return "";
    }
}

string LoadFile(const string& filename) {
    return replaceall(replaceall(read(filename.c_str()), "\r\n", "\n"), "\r", "\n");
}

string GetBinDir() {
    char path[FILENAME_MAX];
#if defined(_WIN32)
    path[GetModuleFileNameA(NULL, path, FILENAME_MAX)] = 0;
#elif defined(__APPLE__)
    unsigned int size = FILENAME_MAX;
    _NSGetExecutablePath(path, &size);
#else
    path[readlink("/proc/self/exe", path, FILENAME_MAX)] = 0;
#endif
    return extractdir(path);
}

string StripLastPathElement(const string& path) {
    if (path == "") {
        return path;
    } else {
        const vector<string> components = split(replaceall(path, "\\", "/"), '/');
        return accumulate(components.begin(), components.end() - 1, string("/")).substr(1);
    }
}

string AppendElementToPath(const string& path, const string& element) {
    const size_t pathSize = path.size();
    return
        (pathSize == 0) ? element :
        (path[pathSize - 1] == '/' || path[pathSize - 1] == '\\') ? (path + element) :
        (path + "/" + element);
}

int main(int argc, char** argv) {
    // Load file
    const string srcFilename = GetFilename(argc, argv);
    const string file = LoadFile(srcFilename);
    if (file == "") Error("Could not load source file or it is empty.");

    // Setup parser
    Parser parser(ParseTokens(file, srcFilename));
    const string basePath = StripLastPathElement(GetBinDir());
    const string coreModuleInfo = LoadFile(AppendElementToPath(basePath, "modules/core/module.txt"));
    parser.ParseLibrary(ParseTokens(coreModuleInfo, "core"));
    
    // Parse
    parser.Parse();

    // Write JS file
    const string dstFilename = stripext(srcFilename) + ".js";
    const string coreModuleJS = LoadFile(AppendElementToPath(basePath, "modules/core/module.js"));
    write(coreModuleJS + "\n" + parser.GetCode(), dstFilename, false);
    return 0;
}
