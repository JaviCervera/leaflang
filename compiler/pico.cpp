#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif
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

int main(int argc, char** argv) {
    const string srcFilename = GetFilename(argc, argv);
    const string dstFilename = stripext(srcFilename) + ".js";
    const string file = replaceall(replaceall(read(srcFilename.c_str()), "\r\n", "\n"), "\r", "\n");
    if (file == "") {
        Error("Could not load source file or it is empty.");
    }
    const vector<Token> tokens = ParseTokens(file, srcFilename);
    Parser parser(tokens);
    AddFunctions(parser);
    parser.Parse();
    write(parser.GetCode(), dstFilename, false);
    return 0;
}
