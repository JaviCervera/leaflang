#pragma once

inline int _lua_AppName(lua_State* L) {
    const char* result = AppName();
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_AppArgs(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int result = AppArgs(a);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_Run(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = Run(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Input(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = Input(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Print(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    Print(a);
    return 0;
}

inline int _lua_DirContents(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    int result = DirContents(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_CurrentDir(lua_State* L) {
    const char* result = CurrentDir();
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_ChangeDir(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    ChangeDir(a);
    return 0;
}

inline int _lua_FullPath(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = FullPath(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_FileType(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    int result = FileType(a);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_DeleteFile(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    DeleteFile(a);
    return 0;
}

inline int _lua_ASin(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = ASin(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_ATan(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = ATan(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_ATan2(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float b = lua_tonumber(L, 2);
    float result = ATan2(a,b);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Abs(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Abs(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Ceil(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Ceil(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Clamp(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float b = lua_tonumber(L, 2);
    float c = lua_tonumber(L, 3);
    float result = Clamp(a,b,c);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Cos(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Cos(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Exp(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Exp(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Floor(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Floor(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Log(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Log(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Max(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float b = lua_tonumber(L, 2);
    float result = Max(a,b);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Min(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float b = lua_tonumber(L, 2);
    float result = Min(a,b);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Pow(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float b = lua_tonumber(L, 2);
    float result = Pow(a,b);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Sgn(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Sgn(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Sin(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Sin(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Sqrt(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Sqrt(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Tan(lua_State* L) {
    float a = lua_tonumber(L, 1);
    float result = Tan(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_Int(lua_State* L) {
    float a = lua_tonumber(L, 1);
    int result = Int(a);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_Dim(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int result = Dim(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_Undim(lua_State* L) {
    int a = lua_tointeger(L, 1);
    Undim(a);
    return 0;
}

inline int _lua_Redim(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    Redim(a,b);
    return 0;
}

inline int _lua_LoadDim(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    int result = LoadDim(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_SaveDim(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    SaveDim(a,b);
    return 0;
}

inline int _lua_DimSize(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int result = DimSize(a);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_PeekByte(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int result = PeekByte(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_PeekShort(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int result = PeekShort(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_PeekInt(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int result = PeekInt(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_PeekFloat(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    float result = PeekFloat(a,b);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_PeekString(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    const char* result = PeekString(a,b);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_PokeByte(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int c = lua_tointeger(L, 3);
    PokeByte(a,b,c);
    return 0;
}

inline int _lua_PokeShort(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int c = lua_tointeger(L, 3);
    PokeShort(a,b,c);
    return 0;
}

inline int _lua_PokeInt(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int c = lua_tointeger(L, 3);
    PokeInt(a,b,c);
    return 0;
}

inline int _lua_PokeFloat(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    float c = lua_tonumber(L, 3);
    PokeFloat(a,b,c);
    return 0;
}

inline int _lua_PokeString(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    const char* c = lua_tostring(L, 3);
    PokeString(a,b,c);
    return 0;
}

inline int _lua_Len(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    int result = Len(a);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_Left(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    int b = lua_tointeger(L, 2);
    const char* result = Left(a,b);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Right(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    int b = lua_tointeger(L, 2);
    const char* result = Right(a,b);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Mid(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    int b = lua_tointeger(L, 2);
    int c = lua_tointeger(L, 3);
    const char* result = Mid(a,b,c);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Lower(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = Lower(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Upper(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = Upper(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Find(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* b = lua_tostring(L, 2);
    int c = lua_tointeger(L, 3);
    int result = Find(a,b,c);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_Replace(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* b = lua_tostring(L, 2);
    const char* c = lua_tostring(L, 3);
    const char* result = Replace(a,b,c);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Trim(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = Trim(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Join(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    const char* result = Join(a,b);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Split(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    const char* c = lua_tostring(L, 3);
    int result = Split(a,b,c);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_StripExt(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = StripExt(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_StripDir(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = StripDir(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_ExtractExt(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = ExtractExt(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_ExtractDir(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = ExtractDir(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Asc(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    int b = lua_tointeger(L, 2);
    int result = Asc(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_Chr(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* result = Chr(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Str(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* result = Str(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_StrF(lua_State* L) {
    float a = lua_tonumber(L, 1);
    const char* result = StrF(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Val(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    int result = Val(a);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_ValF(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    float result = ValF(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_LoadString(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = LoadString(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_SaveString(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* b = lua_tostring(L, 2);
    int c = lua_tointeger(L, 3);
    SaveString(a,b,c);
    return 0;
}

inline int _lua_DimTable(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int result = DimTable(a);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_UndimTable(lua_State* L) {
    int a = lua_tointeger(L, 1);
    UndimTable(a);
    return 0;
}

inline int _lua_SetTableInt(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    int c = lua_tointeger(L, 3);
    SetTableInt(a,b,c);
    return 0;
}

inline int _lua_SetTableFloat(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    float c = lua_tonumber(L, 3);
    SetTableFloat(a,b,c);
    return 0;
}

inline int _lua_SetTableString(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    const char* c = lua_tostring(L, 3);
    SetTableString(a,b,c);
    return 0;
}

inline int _lua_TableInt(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    int result = TableInt(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_TableFloat(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    float result = TableFloat(a,b);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_TableString(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    const char* result = TableString(a,b);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_SetIndexInt(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int c = lua_tointeger(L, 3);
    SetIndexInt(a,b,c);
    return 0;
}

inline int _lua_SetIndexFloat(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    float c = lua_tonumber(L, 3);
    SetIndexFloat(a,b,c);
    return 0;
}

inline int _lua_SetIndexString(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    const char* c = lua_tostring(L, 3);
    SetIndexString(a,b,c);
    return 0;
}

inline int _lua_IndexInt(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    int result = IndexInt(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_IndexFloat(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    float result = IndexFloat(a,b);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_IndexString(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
    const char* result = IndexString(a,b);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Contains(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    int result = Contains(a,b);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_Remove(lua_State* L) {
    int a = lua_tointeger(L, 1);
    const char* b = lua_tostring(L, 2);
    Remove(a,b);
    return 0;
}

inline int _lua_Size(lua_State* L) {
    int a = lua_tointeger(L, 1);
    int result = Size(a);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_Clear(lua_State* L) {
    int a = lua_tointeger(L, 1);
    Clear(a);
    return 0;
}

inline int _lua_AddIntArg(lua_State* L) {
    int a = lua_tointeger(L, 1);
    AddIntArg(a);
    return 0;
}

inline int _lua_AddFloatArg(lua_State* L) {
    float a = lua_tonumber(L, 1);
    AddFloatArg(a);
    return 0;
}

inline int _lua_AddStringArg(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    AddStringArg(a);
    return 0;
}

inline int _lua_Call(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    Call(a);
    return 0;
}

inline int _lua_CallInt(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    int result = CallInt(a);
    lua_pushinteger(L, result);
    return 1;
}

inline int _lua_CallFloat(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    float result = CallFloat(a);
    lua_pushnumber(L, result);
    return 1;
}

inline int _lua_CallString(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    const char* result = CallString(a);
    lua_pushstring(L, result);
    return 1;
}

inline int _lua_Callable(lua_State* L) {
    const char* a = lua_tostring(L, 1);
    int result = Callable(a);
    lua_pushinteger(L, result);
    return 1;
}

static const luaL_Reg picoFuncs[] = {
    {"AppName", _lua_AppName},
    {"AppArgs", _lua_AppArgs},
    {"Run", _lua_Run},
    {"Input", _lua_Input},
    {"Print", _lua_Print},
    {"DirContents", _lua_DirContents},
    {"CurrentDir", _lua_CurrentDir},
    {"ChangeDir", _lua_ChangeDir},
    {"FullPath", _lua_FullPath},
    {"FileType", _lua_FileType},
    {"DeleteFile", _lua_DeleteFile},
    {"ASin", _lua_ASin},
    {"ATan", _lua_ATan},
    {"ATan2", _lua_ATan2},
    {"Abs", _lua_Abs},
    {"Ceil", _lua_Ceil},
    {"Clamp", _lua_Clamp},
    {"Cos", _lua_Cos},
    {"Exp", _lua_Exp},
    {"Floor", _lua_Floor},
    {"Log", _lua_Log},
    {"Max", _lua_Max},
    {"Min", _lua_Min},
    {"Pow", _lua_Pow},
    {"Sgn", _lua_Sgn},
    {"Sin", _lua_Sin},
    {"Sqrt", _lua_Sqrt},
    {"Tan", _lua_Tan},
    {"Int", _lua_Int},
    {"Dim", _lua_Dim},
    {"Undim", _lua_Undim},
    {"Redim", _lua_Redim},
    {"LoadDim", _lua_LoadDim},
    {"SaveDim", _lua_SaveDim},
    {"DimSize", _lua_DimSize},
    {"PeekByte", _lua_PeekByte},
    {"PeekShort", _lua_PeekShort},
    {"PeekInt", _lua_PeekInt},
    {"PeekFloat", _lua_PeekFloat},
    {"PeekString", _lua_PeekString},
    {"PokeByte", _lua_PokeByte},
    {"PokeShort", _lua_PokeShort},
    {"PokeInt", _lua_PokeInt},
    {"PokeFloat", _lua_PokeFloat},
    {"PokeString", _lua_PokeString},
    {"Len", _lua_Len},
    {"Left", _lua_Left},
    {"Right", _lua_Right},
    {"Mid", _lua_Mid},
    {"Lower", _lua_Lower},
    {"Upper", _lua_Upper},
    {"Find", _lua_Find},
    {"Replace", _lua_Replace},
    {"Trim", _lua_Trim},
    {"Join", _lua_Join},
    {"Split", _lua_Split},
    {"StripExt", _lua_StripExt},
    {"StripDir", _lua_StripDir},
    {"ExtractExt", _lua_ExtractExt},
    {"ExtractDir", _lua_ExtractDir},
    {"Asc", _lua_Asc},
    {"Chr", _lua_Chr},
    {"Str", _lua_Str},
    {"StrF", _lua_StrF},
    {"Val", _lua_Val},
    {"ValF", _lua_ValF},
    {"LoadString", _lua_LoadString},
    {"SaveString", _lua_SaveString},
    {"DimTable", _lua_DimTable},
    {"UndimTable", _lua_UndimTable},
    {"SetTableInt", _lua_SetTableInt},
    {"SetTableFloat", _lua_SetTableFloat},
    {"SetTableString", _lua_SetTableString},
    {"TableInt", _lua_TableInt},
    {"TableFloat", _lua_TableFloat},
    {"TableString", _lua_TableString},
    {"SetIndexInt", _lua_SetIndexInt},
    {"SetIndexFloat", _lua_SetIndexFloat},
    {"SetIndexString", _lua_SetIndexString},
    {"IndexInt", _lua_IndexInt},
    {"IndexFloat", _lua_IndexFloat},
    {"IndexString", _lua_IndexString},
    {"Contains", _lua_Contains},
    {"Remove", _lua_Remove},
    {"Size", _lua_Size},
    {"Clear", _lua_Clear},
    {"AddIntArg", _lua_AddIntArg},
    {"AddFloatArg", _lua_AddFloatArg},
    {"AddStringArg", _lua_AddStringArg},
    {"Call", _lua_Call},
    {"CallInt", _lua_CallInt},
    {"CallFloat", _lua_CallFloat},
    {"CallString", _lua_CallString},
    {"Callable", _lua_Callable},
    {NULL, NULL}
};

inline int LoadLuaFunctions(lua_State* L) {
    luaL_newlib(L, picoFuncs);
    lua_setglobal(L, "pico");
    return 1;
}

inline void AddFunctions(Parser& parser) {
    const std::string library = 
        "SUB AppName$()\n"
        "SUB AppArgs%(a)\n"
        "SUB Run$(a$)\n"
        "SUB Input$(a$)\n"
        "SUB Print(a$)\n"
        "SUB DirContents%(a,b$)\n"
        "SUB CurrentDir$()\n"
        "SUB ChangeDir(a$)\n"
        "SUB FullPath$(a$)\n"
        "SUB FileType%(a$)\n"
        "SUB DeleteFile(a$)\n"
        "SUB ASin#(a#)\n"
        "SUB ATan#(a#)\n"
        "SUB ATan2#(a#,b#)\n"
        "SUB Abs#(a#)\n"
        "SUB Ceil#(a#)\n"
        "SUB Clamp#(a#,b#,c#)\n"
        "SUB Cos#(a#)\n"
        "SUB Exp#(a#)\n"
        "SUB Floor#(a#)\n"
        "SUB Log#(a#)\n"
        "SUB Max#(a#,b#)\n"
        "SUB Min#(a#,b#)\n"
        "SUB Pow#(a#,b#)\n"
        "SUB Sgn#(a#)\n"
        "SUB Sin#(a#)\n"
        "SUB Sqrt#(a#)\n"
        "SUB Tan#(a#)\n"
        "SUB Int%(a#)\n"
        "SUB Dim%(a,b)\n"
        "SUB Undim(a)\n"
        "SUB Redim(a,b)\n"
        "SUB LoadDim%(a,b$)\n"
        "SUB SaveDim(a,b$)\n"
        "SUB DimSize%(a)\n"
        "SUB PeekByte%(a,b)\n"
        "SUB PeekShort%(a,b)\n"
        "SUB PeekInt%(a,b)\n"
        "SUB PeekFloat#(a,b)\n"
        "SUB PeekString$(a,b)\n"
        "SUB PokeByte(a,b,c)\n"
        "SUB PokeShort(a,b,c)\n"
        "SUB PokeInt(a,b,c)\n"
        "SUB PokeFloat(a,b,c#)\n"
        "SUB PokeString(a,b,c$)\n"
        "SUB Len%(a$)\n"
        "SUB Left$(a$,b)\n"
        "SUB Right$(a$,b)\n"
        "SUB Mid$(a$,b,c)\n"
        "SUB Lower$(a$)\n"
        "SUB Upper$(a$)\n"
        "SUB Find%(a$,b$,c)\n"
        "SUB Replace$(a$,b$,c$)\n"
        "SUB Trim$(a$)\n"
        "SUB Join$(a,b$)\n"
        "SUB Split%(a,b$,c$)\n"
        "SUB StripExt$(a$)\n"
        "SUB StripDir$(a$)\n"
        "SUB ExtractExt$(a$)\n"
        "SUB ExtractDir$(a$)\n"
        "SUB Asc%(a$,b)\n"
        "SUB Chr$(a)\n"
        "SUB Str$(a)\n"
        "SUB StrF$(a#)\n"
        "SUB Val%(a$)\n"
        "SUB ValF#(a$)\n"
        "SUB LoadString$(a$)\n"
        "SUB SaveString(a$,b$,c)\n"
        "SUB DimTable%(a)\n"
        "SUB UndimTable(a)\n"
        "SUB SetTableInt(a,b$,c)\n"
        "SUB SetTableFloat(a,b$,c#)\n"
        "SUB SetTableString(a,b$,c$)\n"
        "SUB TableInt%(a,b$)\n"
        "SUB TableFloat#(a,b$)\n"
        "SUB TableString$(a,b$)\n"
        "SUB SetIndexInt(a,b,c)\n"
        "SUB SetIndexFloat(a,b,c#)\n"
        "SUB SetIndexString(a,b,c$)\n"
        "SUB IndexInt%(a,b)\n"
        "SUB IndexFloat#(a,b)\n"
        "SUB IndexString$(a,b)\n"
        "SUB Contains%(a,b$)\n"
        "SUB Remove(a,b$)\n"
        "SUB Size%(a)\n"
        "SUB Clear(a)\n"
        "SUB AddIntArg(a)\n"
        "SUB AddFloatArg(a#)\n"
        "SUB AddStringArg(a$)\n"
        "SUB Call(a$)\n"
        "SUB CallInt%(a$)\n"
        "SUB CallFloat#(a$)\n"
        "SUB CallString$(a$)\n"
        "SUB Callable%(a$)\n"
    ;
    parser.ParseLibrary(ParseTokens(library, "library"));
}
