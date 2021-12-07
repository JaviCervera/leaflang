#include "../src/pico.h"
#include "core.h"

using namespace std;

string GenFuncWrappers(const Parser& parser, const string& tableName, const string& funcName);
string GenFuncWrapper(const Function* func);
string GenFuncArgs(const Function* func);
string GenTypeName(int type);
string GenLuaArg(const Var& param);
string GenFuncCall(const Function* func);
string GenLuaReturn(int type);
string GenLuaTable(const Parser& parser, const string& tableName);
string GenLuaRegister(const Parser& parser, const string& tableName, const string& funcName);
string GenLibrary(const Parser& parser, const string& funcName);
string StartLibrary(const string& funcName);
string GenFunction(const Function* func);
string GenTypeSuffix(int type);
string EndLibrary();

int main(int argc, char* argv[]) {
    const string file = Replace(LoadString("corelib.txt"), "\r\n", "\n");
    const vector<Token> tokens = ParseTokens(file, "corelib.txt");
    Parser parser(tokens);
    parser.ParseLibrary(tokens);
    string output = "#pragma once\n\n";
    output += GenFuncWrappers(parser, "picoFuncs", "LoadLuaFunctions");
    output += GenLibrary(parser, "AddFunctions");
    SaveString("corelib.h", output.c_str(), false);
    return 0;
}

string GenFuncWrappers(const Parser& parser, const string& tableName, const string& funcName) {
    string output = "";
    const Lib& lib = parser.GetLib();
    for (size_t i = 0; i < lib.size(); ++i) {
        output += GenFuncWrapper(&lib[i]) + "\n";
    }
    output += GenLuaTable(parser, tableName) + "\n";
    output += GenLuaRegister(parser, tableName, funcName) + "\n";
    return output;
}

string GenFuncWrapper(const Function* func) {
    string result = "inline int _lua_" + func->name + "(lua_State* L) {\n";
    result += GenFuncArgs(func);
    result += GenFuncCall(func);
    result += GenLuaReturn(func->type);
    if (func->type == TYPE_VOID) {
        result += "    return 0;\n";
    } else {
        result += "    return 1;\n";
    }
    result += "}\n";
    return result;
}

string GenFuncArgs(const Function* func) {
    string result = "";
    for (size_t i = 0; i < func->params.size(); ++i) {
        const Var& param = func->params[i];
        result += "    " + GenTypeName(param.type) + Chr(i + 97)
            + " = " + GenLuaArg(param) + "(L, " + Str(i + 1) + ");\n";
    }
    return result;
}

string GenTypeName(int type) {
    switch (type) {
    case TYPE_INT:
        return "int ";
    case TYPE_REAL:
        return "float ";
    case TYPE_STRING:
        return "const char* ";
    case TYPE_HASH:
        return "Hash* ";
    case TYPE_REF:
        return "void* ";
    default:
        return "";
    }
}

string GenLuaArg(const Var& param) {
    switch (param.type) {
    case TYPE_INT:
        return "lua_tointeger";
    case TYPE_REAL:
        return "lua_tonumber";
    case TYPE_STRING:
        return "lua_tostring";
    case TYPE_HASH:
        return "(Hash*)lua_topointer";
    case TYPE_REF:
        return "(void*)lua_topointer";
    default:
        return "";
    }
}

string GenFuncCall(const Function* func) {
    string result = "    ";
    if (func->type != TYPE_VOID) {
        result += GenTypeName(func->type) + "result = ";
    }
    result += func->name + "(";
    for (size_t i = 0; i < func->params.size(); ++i) {
        result += Chr(i + 97);
        if (i < func->params.size() - 1) result += ",";
    }
    result += ");\n";
    return result;
}

string GenLuaReturn(int type) {
    switch (type) {
    case TYPE_INT:
        return "    lua_pushinteger(L, result);\n";
    case TYPE_REAL:
        return "    lua_pushnumber(L, result);\n";
    case TYPE_STRING:
        return "    lua_pushstring(L, result);\n";
    case TYPE_HASH:
        return "    lua_pushlightuserdata(L, result);\n";
    case TYPE_REF:
        return "    lua_pushlightuserdata(L, result);\n";
    default:
        return "";
    }
}

string GenLuaTable(const Parser& parser, const string& tableName) {
    string result = "static const luaL_Reg " + tableName + "[] = {\n";
    const Lib& lib = parser.GetLib();
    for (size_t i = 0; i < lib.size(); ++i) {
        const Function& func = lib[i];
        result += "    {\"" + func.name + "\", _lua_" + func.name + "},\n";
    }
    result += "    {NULL, NULL}\n";
    result += "};\n";
    return result;
}

string GenLuaRegister(const Parser& parser, const string& tableName, const string& funcName) {
    string result = "inline int " + funcName + "(lua_State* L) {\n";
    result += "    luaL_newlib(L, " + tableName + ");\n";
    result += "    lua_setglobal(L, \"pico\");\n";
    result += "    return 1;\n";
    result += "}\n";
    return result;
}

string GenLibrary(const Parser& parser, const string& funcName) {
    string output = StartLibrary(funcName);
    const Lib& lib = parser.GetLib();
    for (size_t i = 0; i < lib.size(); ++i) {
        if (lib[i].name[0] != '_') {
            output += GenFunction(&lib[i]);
        }
    }
    output += EndLibrary();
    return output;
}

string StartLibrary(const string& funcName) {
    string result = "inline void " + funcName + "(Parser& parser) {\n";
    result += "    const std::string library = \n";
    return result;
}

string GenFunction(const Function* func) {
    string result = "        \"function ";
    result += func->name;
    result += GenTypeSuffix(func->type) + "(";
    for (size_t i = 0; i < func->params.size(); ++i) {
        result += Chr(i + 97) + GenTypeSuffix(func->params[i].type);
        if (i < func->params.size() - 1) result += ",";
    }
    result += ")\\n\"\n";
    return result;
}

string GenTypeSuffix(int type) {
    switch (type) {
    case TYPE_INT:
        return "|i";
    case TYPE_REAL:
        return "|f";
    case TYPE_STRING:
        return "|s";
    case TYPE_HASH:
        return "|h";
    case TYPE_REF:
        return "|w";
    default:
        return "";
    }
}

string EndLibrary() {
    string result = "    ;\n";
    result += "    parser.ParseLibrary(ParseTokens(library, \"library\"));\n";
    result += "}\n";
    return result;
}
