#include <cmath>
#include "core.h"
#include "../src/swan/console.hh"
#include "../src/swan/dir.hh"
#include "../src/swan/file.hh"
#include "../src/swan/platform.hh"
#include "../src/swan/strmanip.hh"

using namespace std;
using namespace swan;

#define DEG2RAD 0.01745329f
#define RAD2DEG 57.29578f

struct Any {
    int i;
    float f;
    std::string s;
    void* r;
    int type;

    Any(int number = 0) : i(number), type(TYPE_INT) {}
    Any(float number) : f(number), type(TYPE_REAL) { }
    Any(const char* str) : s(str), type(TYPE_STRING) {}
    Any(const std::string& str) : s(str), type(TYPE_STRING) {}
#ifdef ENABLE_REF
    Any(void* ref) : r(ref), type(TYPE_REF) {}
#endif
    Any(const Any& other) { *this = other; }
    
    int ToInt() const;
    float ToReal() const;
    std::string ToString() const;
#ifdef ENABLE_REF
    void* ToRef() const;
#endif
};

struct Table : public map<string, Any> {
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
    default: return "";
    }
}

#ifdef ENABLE_REF
void* Any::ToRef() const {
    switch (type) {
    case TYPE_INT: return NULL;
    case TYPE_REAL: return NULL;
    case TYPE_STRING: return (void*)s.c_str();
    default: return r;
    }
}
#endif

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

// ------------------------------------
// App
// ------------------------------------

#define APPARGS_TABLE 65535

static string pico_appName;
static Table pico_appArgs;

void _SetArgs(int argc, const char* argv[]) {
    pico_appName = argv[0];
    for (int i = 1; i < argc; ++i) {
        pico_appArgs[strmanip::fromint(pico_appArgs.size())] = argv[i];
    }
}

const char* AppName() {
    return pico_appName.c_str();
}

size_t AppArgs(size_t table) {
    table = DimTable(table);
    for (int i = 0; i < pico_appArgs.size(); ++i) {
        const string index = strmanip::fromint(i);
        SetTableString(table, index.c_str(), pico_appArgs[index].s.c_str());
    }
    return table;
}

const char* Run(const char* command) {
    static string result;
    result = platform::run(command);
    return result.c_str();
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

size_t DirContents(size_t table, const char* path) {
    const vector<string> contents = dir::contents(path);
    table = DimTable(table);
    int i = 0;
    for (vector<string>::const_iterator it = contents.begin(); it != contents.end(); ++it) {
        SetTableString(table, strmanip::fromint(i++).c_str(), (*it).c_str());
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
    return asin(x) * RAD2DEG;
}

float ATan(float x) {
    return atan(x) * RAD2DEG;
}

float ATan2(float x, float y) {
    return atan2(x, y) * RAD2DEG;
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
    return cos(x * DEG2RAD);
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
    return sin(x * DEG2RAD);
}

float Sqrt(float x) {
    return sqrt(x);
}

float Tan(float x) {
    return tan(x * DEG2RAD);
}

int Int(float num) {
    return int(num);
}

// ------------------------------------
// Memory
// ------------------------------------

struct Memblock {
    char* ptr;
    size_t size;

    Memblock(size_t size) : size(size) { ptr = (char*)calloc(1, size); }

    ~Memblock() { free(this->ptr); }

    void Resize(size_t size) { ptr = (char*)realloc(ptr, size); this->size = size; }
};

static void DeleteMem(Memblock* mem) {
    delete mem;
}

static Pool<Memblock*> _memPool(DeleteMem);

size_t Dim(size_t index, int size) {
    return _memPool.Insert(index, new Memblock(size));
}

void Undim(size_t index) {
    _memPool.Remove(index);
}

void Redim(size_t index, int size) {
    _memPool.Get(index)->Resize(size);
}

size_t LoadDim(size_t index, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    size_t size = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    index = Dim(index, size);
    fread(_memPool.Get(index)->ptr, size, 1, f);
    fclose(f);
    return index;
}

void SaveDim(size_t index, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    fwrite(_memPool.Get(index)->ptr, DimSize(index), 1, f);
    fclose(f);
}

int DimSize(size_t index) {
    return (int)_memPool.Get(index)->size;
}

int PeekByte(size_t index, int offset) {
    unsigned char v;
    memcpy(&v, &_memPool.Get(index)->ptr[offset], sizeof(v));
    return (int)v;
}

int PeekShort(size_t index, int offset) {
    unsigned short v;
    memcpy(&v, &_memPool.Get(index)->ptr[offset], sizeof(v));
    return (int)v;
}

int PeekInt(size_t index, int offset) {
    int v;
    memcpy(&v, &_memPool.Get(index)->ptr[offset], sizeof(v));
    return v;
}

float PeekFloat(size_t index, int offset) {
    float v;
    memcpy(&v, &_memPool.Get(index)->ptr[offset], sizeof(v));
    return v;
}

const char* PeekString(size_t index, int offset) {
    static string result;
    int c;
    while (offset < DimSize(index) && (c = PeekByte(index, offset)) != 0) {
        result += (char)c;
        ++offset;
    }
    return result.c_str();
}

void PokeByte(size_t index, int offset, int val) {
    unsigned char* b = (unsigned char*)&val;
    memcpy(&(_memPool.Get(index)->ptr[offset]), &b[3], sizeof(unsigned char));
}

void PokeShort(size_t index, int offset, int val) {
    unsigned short* s = (unsigned short*)&val;
    memcpy(&(_memPool.Get(index)->ptr[offset]), &s[1], sizeof(unsigned short));
}

void PokeInt(size_t index, int offset, int val) {
    memcpy(&(_memPool.Get(index)->ptr[offset]), &val, sizeof(val));
}

void PokeFloat(size_t index, int offset, float val) {
    memcpy(&(_memPool.Get(index)->ptr[offset]), &val, sizeof(val));
}

void PokeString(size_t index, int offset, const char* val) {
    memcpy(&(_memPool.Get(index)->ptr[offset]), val, strlen(val) + 1);
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

const char* Join(size_t table, const char* separator) {
    static string result;
    result = "";
    const int size = Size(table);
    for (int i = 0; i < size; ++i) {
        if (i > 0) result += separator;
        result += TableString(table, strmanip::fromint(i).c_str());
    }
    return result.c_str();
}

size_t Split(size_t table, const char* str, const char* separator) {
    const char delim = (Len(separator) > 0) ? separator[0] : ' ';
    const vector<string> split = strmanip::split(str, delim);
    table = DimTable(table);
    int i = 0;
    for (vector<string>::const_iterator it = split.begin(); it != split.end(); ++it) {
        SetTableString(table, strmanip::fromint(i++).c_str(), (*it).c_str());
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

static void DeleteTable(Table* table) {
    delete table;
}

static Pool<Table*> _tablePool(DeleteTable);

size_t DimTable(size_t index) {
    return _tablePool.Insert(index, new Table());
}

void UndimTable(size_t table) {
    _tablePool.Remove(table);
}

void SetTableInt(size_t table, const char* key, int value) {
    (*_tablePool.Get(table))[key] = value;
}

void SetTableFloat(size_t table, const char* key, float value) {
    (*_tablePool.Get(table))[key] = value;
}

void SetTableString(size_t table, const char* key, const char* value) {
    (*_tablePool.Get(table))[key] = value;
}

int TableInt(const size_t table, const char* key) {
    return (Contains(table, key))
        ? (*_tablePool.Get(table))[key].i
        : 0;
}

float TableFloat(const size_t table, const char* key) {
    return (Contains(table, key))
        ? (*_tablePool.Get(table))[key].f
        : 0.0f;
}

const char* TableString(const size_t table, const char* key) {
    return (Contains(table, key))
        ? (*_tablePool.Get(table))[key].s.c_str()
        : "";
}

void SetIndexInt(size_t table, size_t index, int value) {
    (*_tablePool.Get(table))[Str(index)] = value;
}

void SetIndexFloat(size_t table, size_t index, float value) {
    (*_tablePool.Get(table))[Str(index)] = value;
}

void SetIndexString(size_t table, size_t index, const char* value) {
    (*_tablePool.Get(table))[Str(index)] = value;
}

int IndexInt(const size_t table, size_t index) {
    return (Contains(table, Str(index)))
        ? (*_tablePool.Get(table))[Str(index)].i
        : 0;
}

float IndexFloat(const size_t table, size_t index) {
    return (Contains(table, Str(index)))
        ? (*_tablePool.Get(table))[Str(index)].f
        : 0.0f;
}

const char* IndexString(const size_t table, size_t index) {
    return (Contains(table, Str(index)))
        ? (*_tablePool.Get(table))[Str(index)].s.c_str()
        : "";
}

int Contains(const size_t table, const char* key) {
    return _tablePool.Get(table)->count(key) > 0;
}

void Remove(size_t table, const char* key) {
    if (Contains(table, key)) {
        _tablePool.Get(table)->erase(key);
    }
}

int Size(const size_t table) {
    return _tablePool.Get(table)->size();
}

void Clear(size_t table) {
    return _tablePool.Get(table)->clear();
}

// ------------------------------------
// Callable
// ------------------------------------

void AddIntArg(int arg) {
}

void AddFloatArg(float arg) {
}

void AddStringArg(const char* arg) {
}

void Call(const char* name) {
}

int CallInt(const char* name) {
    return 0;
}

float CallFloat(const char* name) {
    return 0.0f;
}

const char* CallString(const char* name) {
    static const char* str = "";
    return str;
}

int Callable(const char* name) {
    return 0;
}
