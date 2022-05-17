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

int main(int argc, char** argv) {
    const string filename = GetFilename(argc, argv);
    const string file = Replace(Replace(LoadString(filename.c_str()), "\r\n", "\n"), "\r", "\n");
    if (file == "") Error("Could not load source file or it is empty.");
    const string prevDir = CurrentDir();
    ChangeDir(AppDir());
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
    _DoAutoDec();

    return 0;
}
