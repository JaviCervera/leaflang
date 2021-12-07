#include <cmath>
#define CORE_IMPL
#include "core.h"
#include "../src/swan/console.hh"
#include "../src/swan/dir.hh"
#include "../src/swan/file.hh"
#include "../src/swan/platform.hh"
#include "../src/swan/strmanip.hh"
#define LITE_MEM_IMPLEMENTATION
#include "litemem.h"

using namespace std;
using namespace swan;

struct Any {
    int i;
    float f;
    std::string s;
    Table* t;
    void* r;
    int type;

    Any(int number = 0) : i(number), type(TYPE_INT) {}
    Any(float number) : f(number), type(TYPE_REAL) { }
    Any(const char* str) : s(str), type(TYPE_STRING) {}
    Any(const std::string& str) : s(str), type(TYPE_STRING) {}
    Any(Table* table) : t(table), type(TYPE_TABLE) {}
    Any(void* ref) : r(ref), type(TYPE_REF) {}
    Any(const Any& other) { *this = other; }
    
    int ToInt() const;
    float ToReal() const;
    std::string ToString() const;
    Table* ToTable() const;
    void* ToRef() const;
};

struct Table : public map<string, Any> {
    ~Table();
    string ToString();
};

int Any::ToInt() const {
    switch (type) {
    case TYPE_INT: return i;
    case TYPE_REAL: return (int)f;
    case TYPE_STRING: return strmanip::toint(s);
    default: return 0;
    }
}

float Any::ToReal() const {
    switch (type) {
    case TYPE_INT: return i;
    case TYPE_REAL: return f;
    case TYPE_STRING: return strmanip::tofloat(s);
    default: return 0.0f;
    }
}

string Any::ToString() const {
    switch (type) {
    case TYPE_INT: return strmanip::fromint(i);
    case TYPE_REAL: return strmanip::fromdouble(f);
    case TYPE_STRING: return s;
    case TYPE_TABLE: return t->ToString();
    default: return "";
    }
}

Table* Any::ToTable() const {
    switch (type) {
    case TYPE_INT: return NULL;
    case TYPE_REAL: return NULL;
    case TYPE_STRING: return NULL;
    case TYPE_REF: return (Table*)r;
    default: return t;
    }
}

void* Any::ToRef() const {
    switch (type) {
    case TYPE_INT: return NULL;
    case TYPE_REAL: return NULL;
    case TYPE_STRING: return (void*)s.c_str();
    case TYPE_TABLE: return t;
    default: return r;
    }
}

Table::~Table() {
    for (Table::iterator it = begin(); it != end(); ++it) {
        if (it->second.type == TYPE_TABLE) {
            _DecRef(it->second.t);
        }
    }
}

string Table::ToString() {
    string content;
    for (Table::iterator it = begin(); it != end(); ++it) {
        const string prefix = (it->second.type == TYPE_STRING)
            ? "\""
            : "";
        if (it != begin()) content += ", ";
        content += "\"" + it->first + "\": " + prefix + it->second.ToString() + prefix;
    }
    return "{" + content + "}";
}

extern "C" {

// ------------------------------------
// App
// ------------------------------------

static string pico_appName;
static Table* pico_appArgs = (Table*)_IncRef(_CreateTable());

void _SetArgs(int argc, const char* argv[]) {
    pico_appName = argv[0];
    for (int i = 1; i < argc; ++i) {
        _SetTableString(pico_appArgs, Str(i - 1), argv[i]);
    }
}

const char* AppName() {
    return pico_appName.c_str();
}

Table* AppArgs() {
    return pico_appArgs;
}

const char* Run(const char* command) {
    static string result;
    result = platform::run(command);
    return result.c_str();
}

void* _IncRef(void* ptr) {
    lmem_retain(ptr);
    return ptr;
}

void _DecRef(void* ptr) {
    lmem_release(ptr);
}

void* _AutoDec(void* ptr) {
    return lmem_autorelease(ptr);
}

void _DoAutoDec() {
    lmem_doautorelease();
}

// ------------------------------------
// Console
// ------------------------------------

const char* Input(const char* prompt) {
    static string result;
    result = Replace(console::input(prompt).c_str(), "\n", "");
    return result.c_str();
}

void Print(const char* msg) {
    console::println(msg);
}

// ------------------------------------
// Dir
// ------------------------------------

Table* DirContents(const char* path) {
    const vector<string> contents = dir::contents(path);
    Table* table = _CreateTable();
    int i = 0;
    for (vector<string>::const_iterator it = contents.begin(); it != contents.end(); ++it) {
        _SetTableString(table, Str(i++), (*it).c_str());
    }
    return table;
}

const char* CurrentDir() {
    static string result;
    result = dir::current();
    return result.c_str();
}

void ChangeDir(const char* dir) {
    dir::change(dir);
}

const char* FullPath(const char* filename) {
    static string result;
    result = dir::real_path(filename);
    return result.c_str();
}

// ------------------------------------
// File
// ------------------------------------

int FileType(const char* filename) {
    return filetype(filename);
}

void DeleteFile(const char* filename) {
    return fileremove(filename);
}

// ------------------------------------
// Math
// ------------------------------------

float ASin(float x) {
    return asin(x);
}

float ATan(float x) {
    return atan(x);
}

float ATan2(float x, float y) {
    return atan2(x, y);
}

float Abs(float x) {
    return abs(x);
}

float Ceil(float x) {
    return ceil(x);
}

float Clamp(float x, float min, float max) {
    return Min(Max(x, min), max);
}

float Cos(float x) {
    return cos(x);
}

float Exp(float x) {
    return exp(x);
}

float Floor(float x) {
    return floor(x);
}

float Log(float x) {
    return log(x);
}

float Max(float x, float y) {
    return max(x, y);
}

float Min(float x, float y) {
    return min(x, y);
}

float Pow(float x, float y) {
    return pow(x, y);
}

float Sgn(float x) {
    return (0 < x) - (x < 0);
}

float Sin(float x) {
    return sin(x);
}

float Sqrt(float x) {
    return sqrt(x);
}

float Tan(float x) {
    return tan(x);
}

int Int(float num) {
    return int(num);
}

// ------------------------------------
// Memory
// ------------------------------------

struct Memory {
    char* ptr;
    size_t size;

    Memory(size_t size) : size(size) { ptr = (char*)calloc(1, size); }
    ~Memory() { free(this->ptr); }
    void Resize(size_t size) { ptr = (char*)realloc(ptr, size); this->size = size; }
};

Memory* Dim(int size) {
    return new Memory(size);
}

void Undim(Memory* mem) {
    delete mem;
}

void Redim(Memory* mem, int size) {
    mem->Resize(size);
}

Memory* LoadDim(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    size_t size = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    Memory* mem = Dim(size);
    fread(mem->ptr, size, 1, f);
    fclose(f);
    return mem;
}

void SaveDim(Memory* mem, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    fwrite(mem->ptr, DimSize(mem), 1, f);
    fclose(f);
}

int DimSize(Memory* mem) {
    return (int)mem->size;
}

int PeekByte(Memory* mem, int offset) {
    unsigned char v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return (int)v;
}

int PeekShort(Memory* mem, int offset) {
    unsigned short v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return (int)v;
}

int PeekInt(Memory* mem, int offset) {
    int v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return v;
}

float PeekReal(Memory* mem, int offset) {
    float v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return v;
}

const char* PeekString(Memory* mem, int offset) {
    static string result;
    int c;
    while (offset < DimSize(mem) && (c = PeekByte(mem, offset)) != 0) {
        result += (char)c;
        ++offset;
    }
    return result.c_str();
}

void* PeekRef(Memory* mem, int offset) {
    void* v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return v;
}

void PokeByte(Memory* mem, int offset, int val) {
    unsigned char* b = (unsigned char*)&val;
    memcpy(&(mem->ptr[offset]), &b[3], sizeof(unsigned char));
}

void PokeShort(Memory* mem, int offset, int val) {
    unsigned short* s = (unsigned short*)&val;
    memcpy(&(mem->ptr[offset]), &s[1], sizeof(unsigned short));
}

void PokeInt(Memory* mem, int offset, int val) {
    memcpy(&(mem->ptr[offset]), &val, sizeof(val));
}

void PokeReal(Memory* mem, int offset, float val) {
    memcpy(&(mem->ptr[offset]), &val, sizeof(val));
}

void PokeString(Memory* mem, int offset, const char* val) {
    memcpy(&(mem->ptr[offset]), val, strlen(val) + 1);
}

void PokeRef(Memory* mem, int offset, void* val) {
    memcpy(&(mem->ptr[offset]), &val, sizeof(val));
}

// ------------------------------------
// String
// ------------------------------------

int Len(const char* str) {
    return string(str).length();
}

const char* Left(const char* str, int count) {
    static string result;
    result = string(str).substr(0, count);
    return result.c_str();
}

const char* Right(const char* str, int count) {
    static string result;
    result = string(str).substr(Len(str) - count);
    return result.c_str();
}

const char* Mid(const char* str, int offset, int count) {
    static string result;
    result = string(str).substr(offset, count);
    return result.c_str();
}

const char* Lower(const char* str) {
    static string result;
    result = strmanip::lower(str);
    return result.c_str();
}

const char* Upper(const char* str) {
    static string result;
    result = strmanip::upper(str);
    return result.c_str();
}

int Find(const char* str, const char* find, int offset) {
    return int(string(str).find(find, offset));
}

const char* Replace(const char* str, const char* find, const char* replace) {
    static string result;
    result = strmanip::replaceall(str, find, replace);
    return result.c_str();
}

const char* Trim(const char* str) {
    static string result;
    result = strmanip::trim(str);
    return result.c_str();
}

const char* Join(Table* table, const char* separator) {
    static string result;
    result = "";
    const int size = Size(table);
    for (int i = 0; i < size; ++i) {
        if (i > 0) result += separator;
        result += _TableString(table, Str(i));
    }
    return result.c_str();
}

Table* Split(const char* str, const char* separator) {
    const char delim = (Len(separator) > 0) ? separator[0] : ' ';
    const vector<string> split = strmanip::split(str, delim);
    Table* table = _CreateTable();
    int i = 0;
    for (vector<string>::const_iterator it = split.begin(); it != split.end(); ++it) {
        _SetTableString(table, Str(i++), (*it).c_str());
    }
    return table;
}

const char* StripExt(const char* filename) {
    static string result;
    result = strmanip::stripext(filename);
    return result.c_str();
}

const char* StripDir(const char* filename) {
    static string result;
    result = strmanip::stripdir(filename);
    return result.c_str();
}

const char* ExtractExt(const char* filename) {
    static string result;
    result = strmanip::extractext(filename);
    return result.c_str();
}

const char* ExtractDir(const char* filename) {
    static string result;
    result = strmanip::extractdir(filename);
    return result.c_str();
}

int Asc(const char* str, int index) {
    return int(str[index]);
}

const char* Chr(int c) {
    static string result;
    result = string(1, char(c));
    return result.c_str();
}

const char* Str(int val) {
    static string result;
    result = strmanip::fromint(val);
    return result.c_str();
}

const char* StrF(float val) {
    static string result;
    result = strmanip::fromdouble(val);
    return result.c_str();
}

int Val(const char* str) {
    return strmanip::toint(str);
}

float ValF(const char* str) {
    return strmanip::tofloat(str);
}

const char* LoadString(const char* filename) {
    static string result;
    result = strmanip::read(filename);
    return result.c_str();
}

void SaveString(const char* filename, const char* str, int append) {
    strmanip::write(str, filename, append);
}

// ------------------------------------
// Table
// ------------------------------------

void _DestroyTable(Table* table) {
    table->~Table();
}

Table* _CreateTable() {
    Table* table = lmem_allocauto(Table, (void*)_DestroyTable);
    new (table) Table();
    return table;
}

void _ClearTableKey(Table* table, const char* key) {
    if (Contains(table, key) && (*(Table*)table)[key].type == TYPE_TABLE) {
        _DecRef((*(Table*)table)[key].t);
    }
}

Table* _SetTableInt(Table* table, const char* key, int value) {
    _ClearTableKey(table, key);
    (*table)[key] = value;
    return table;
}

Table* _SetTableReal(Table* table, const char* key, float value) {
    _ClearTableKey(table, key);
    (*table)[key] = value;
    return table;
}

Table* _SetTableString(Table* table, const char* key, const char* value) {
    _ClearTableKey(table, key);
    (*table)[key] = value;
    return table;
}

Table* _SetTableTable(Table* table, const char* key, Table* value) {
    _IncRef(value);
    _ClearTableKey(table, key);
    (*table)[key] = value;
    return table;
}

Table* _SetTableRef(Table* table, const char* key, void* value) {
    _ClearTableKey(table, key);
    (*table)[key] = value;
    return table;
}

int _TableInt(const Table* table, const char* key) {
    return (Contains(table, key))
        ? (*(Table*)table)[key].i
        : 0;
}

float _TableReal(const Table* table, const char* key) {
    return (Contains(table, key))
        ? (*(Table*)table)[key].f
        : 0.0f;
}

const char* _TableString(const Table* table, const char* key) {
    return (Contains(table, key))
        ? (*(Table*)table)[key].s.c_str()
        : "";
}

Table* _TableTable(const Table* table, const char* key) {
    return (Contains(table, key))
        ? (*(Table*)table)[key].t
        : NULL;
}

void* _TableRef(const Table* table, const char* key) {
    return (Contains(table, key))
        ? (*(Table*)table)[key].r
        : NULL;
}

int Contains(const Table* table, const char* key) {
    return table->count(key) > 0;
}

void Remove(Table* table, const char* key) {
    if (Contains(table, key)) {
        table->erase(key);
    }
}

int Size(const Table* table) {
    return table->size();
}

void Clear(Table* table) {
    return table->clear();
}

// ------------------------------------
// Callable
// ------------------------------------

void AddIntArg(int arg) {
}

void AddRealArg(float arg) {
}

void AddStringArg(const char* arg) {
}

void Call(const char* name) {
}

int CallInt(const char* name) {
    return 0;
}

float CallReal(const char* name) {
    return 0.0f;
}

const char* CallString(const char* name) {
    static const char* str = "";
    return str;
}

int Callable(const char* name) {
    return 0;
}

}
