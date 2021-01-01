#include "../src/pico.h"
#include "../src/swan/platform.hh"
#include "core.h"
#include "lua/lua.hpp"
#include "corelib.h"

using namespace std;

void Run(const string& code);

string GetFilename(int argc, char** argv) {
    if (argc >= 2) {
        return argv[argc-1];
    } else {
        Error("No input file specified.");
        return "";
    }
}

int main(int argc, char** argv) {
    const string filename = GetFilename(argc, argv);
    const string file = Replace(LoadString(filename.c_str()), "\r\n", "\n");
    if (file == "") {
        Error("Could not load source file or it is empty.");
    }
    const vector<Token> tokens = ParseTokens(file, filename);
    Parser parser(tokens);
    AddFunctions(parser);
    parser.Parse();
    Run(parser.GetCode());
    return 0;
}

void Run(const string& code) {
    lua_State* L = luaL_newstate();
    luaL_requiref(L, "_G", luaopen_base, true); lua_pop(L, 1);
    luaL_requiref(L, "table", luaopen_table, true); lua_pop(L, 1);
    LoadLuaFunctions(L);
    if (luaL_dostring(L, code.c_str())) {
        Error(lua_tostring(L, -1));
    }
}
