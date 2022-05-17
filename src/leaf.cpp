#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef LoadString
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif
#include "leaf.h"
#include "swan/platform.hh"
#include "../_build/libs/core/core.h"

using namespace std;

static string GetFilename(int argc, char** argv) {
    if (argc >= 2) {
        return argv[argc-1];
    } else {
        Error("No input file specified.");
        return "";
    }
}

static string GetBinDir() {
    char path[FILENAME_MAX];
#if defined(_WIN32)
    path[GetModuleFileNameA(NULL, path, FILENAME_MAX)] = 0;
#elif defined(__APPLE__)
    unsigned int size = FILENAME_MAX;
    _NSGetExecutablePath(path, &size);
#else
    path[readlink("/proc/self/exe", path, FILENAME_MAX)] = 0;
#endif
    return ExtractDir(path);
}

static string GetRootDir() {
    const string path = Replace(GetBinDir().c_str(), "\\", "/");
    struct TList* components = Split(path.c_str(), "/");
    RemoveIndex(components, ListSize(components) - 1);
    const string str = Join(components, "/");
    _DoAutoDec();
    return str;
}

int main(int argc, char** argv) {
    const string filename = GetFilename(argc, argv);
    const string file = Replace(Replace(LoadString(filename.c_str()), "\r\n", "\n"), "\r", "\n");
    if (file == "") Error("Could not load source file or it is empty.");
    const string prevDir = CurrentDir();
    ChangeDir(GetBinDir().c_str());
    const string lib = Replace(Replace(LoadString("../libs/core/core.lb"), "\r\n", "\n"), "\r", "\n");
    if (lib == "") Error("Could not load core module or it is empty.");
    ChangeDir(prevDir.c_str());
    _DoAutoDec();

    const vector<Token> tokens = ParseTokens(file, filename);
    Parser parser(tokens);
    parser.ParseLibrary(ParseTokens(lib, "library"));
    parser.Parse();
    
    const string outFilename = string(StripExt(filename.c_str())) + ".c";
    SaveString(outFilename.c_str(), parser.GetCode().c_str(), false);

    const string rootDir = GetRootDir();
#ifdef _WIN32
    const string binFilename = string(StripExt(filename.c_str())) + ".exe";
#else
    const string binFilename = StripExt(filename.c_str());
#endif
    const TInt result = System((
        string("gcc")
        + " -o \"" + binFilename + "\""
        + " \"" + outFilename + "\""
        + " \"" + rootDir + "/libs/core/core.c\""
        + " -I\"" + rootDir + "/libs\""
        + " -w -lm -O2 -s"
        ).c_str());
    
    if (result == 0) {
#ifdef _WIN32
        const string command = binFilename;
#else
        const string command = (string(ExtractDir(binFilename.c_str())) == "")
            ? ("./" + binFilename)
            : binFilename;
#endif
        System(command.c_str());
        DeleteFile(binFilename.c_str());
        DeleteFile(outFilename.c_str());
    }
    
    _DoAutoDec();
    return 0;
}
