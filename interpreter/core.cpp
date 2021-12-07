#include <cmath>
#include <map>
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

typedef struct {
    int type;
    union {
        int i;
        float f;
        char* s;
        struct Hash* t;
        void* r;
    } value;
} Value;

struct Hash : public map<string, Value> {
    ~Hash();
    string ToString();
};

Value ValueFromInt(int i) {
    Value v = {0};
    v.type = TYPE_INT;
    v.value.i = i;
    return v;
}

Value ValueFromReal(float f) {
    Value v = {0};
    v.type = TYPE_REAL;
    v.value.f = f;
    return v;
}

Value ValueFromString(const char* s) {
    Value v = {0};
    v.type = TYPE_STRING;
    lmem_assign(v.value.s, (char*)lstr_get(s));
    return v;
}

Value ValueFromHash(struct Hash* t) {
    Value v = {0};
    v.type = TYPE_HASH;
    lmem_assign(v.value.t, t);
    return v;
}

Value ValueFromRef(void* r) {
    Value v = {0};
    v.type = TYPE_REF;
    v.value.r = r;
    return v;
}

int ValueToInt(const Value v) {
    switch (v.type) {
    case TYPE_INT: return v.value.i;
    case TYPE_REAL: return (int)v.value.f;
    case TYPE_STRING: return Val(v.value.s);
    default: return 0;
    }
}

float ValueToReal(const Value v) {
    switch (v.type) {
    case TYPE_INT: return v.value.i;
    case TYPE_REAL: return v.value.f;
    case TYPE_STRING: return ValF(v.value.s);
    default: return 0.0f;
    }
}

const char* ValueToString(const Value v) {
    switch (v.type) {
    case TYPE_INT: return Str(v.value.i);
    case TYPE_REAL: return StrF(v.value.f);
    case TYPE_STRING: return v.value.s;
    case TYPE_HASH: return lstr_get(v.value.t->ToString().c_str());
    default: return lstr_get("");
    }
}

Hash* ValueToHash(const Value v) {
    switch (v.type) {
    case TYPE_INT: return NULL;
    case TYPE_REAL: return NULL;
    case TYPE_STRING: return NULL;
    case TYPE_REF: return (Hash*)v.value.r;
    default: return v.value.t;
    }
}

void* ValueToRef(const Value v) {
    switch (v.type) {
    case TYPE_INT: return NULL;
    case TYPE_REAL: return NULL;
    case TYPE_STRING: return v.value.s;
    case TYPE_HASH: return v.value.t;
    default: return v.value.r;
    }
}

Hash::~Hash() {
    for (Hash::iterator it = begin(); it != end(); ++it) {
        if (it->second.type == TYPE_HASH) {
            _DecRef(it->second.value.t);
        }
    }
}

string Hash::ToString() {
    string content;
    for (Hash::iterator it = begin(); it != end(); ++it) {
        const string prefix = (it->second.type == TYPE_STRING)
            ? "\""
            : "";
        if (it != begin()) content += ", ";
        content += "\"" + it->first + "\": " + prefix + ValueToString(it->second) + prefix;
    }
    return "{" + content + "}";
}

extern "C" {

// ------------------------------------
// App
// ------------------------------------

static string pico_appName;
static Hash* pico_appArgs = (Hash*)_IncRef(_CreateHash());

void _SetArgs(int argc, const char* argv[]) {
    pico_appName = argv[0];
    for (int i = 1; i < argc; ++i) {
        _SetHashString(pico_appArgs, Str(i - 1), argv[i]);
    }
}

const char* AppName() {
    return pico_appName.c_str();
}

Hash* AppArgs() {
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

Hash* DirContents(const char* path) {
    const vector<string> contents = dir::contents(path);
    Hash* hash = _CreateHash();
    int i = 0;
    for (vector<string>::const_iterator it = contents.begin(); it != contents.end(); ++it) {
        _SetHashString(hash, Str(i++), (*it).c_str());
    }
    return hash;
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

const char* Join(Hash* hash, const char* separator) {
    static string result;
    result = "";
    const int size = Size(hash);
    for (int i = 0; i < size; ++i) {
        if (i > 0) result += separator;
        result += _HashString(hash, Str(i));
    }
    return result.c_str();
}

Hash* Split(const char* str, const char* separator) {
    const char delim = (Len(separator) > 0) ? separator[0] : ' ';
    const vector<string> split = strmanip::split(str, delim);
    Hash* hash = _CreateHash();
    int i = 0;
    for (vector<string>::const_iterator it = split.begin(); it != split.end(); ++it) {
        _SetHashString(hash, Str(i++), (*it).c_str());
    }
    return hash;
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
// Hash
// ------------------------------------

void _DestroyHash(Hash* hash) {
    hash->~Hash();
}

Hash* _CreateHash() {
    Hash* hash = lmem_allocauto(Hash, (void*)_DestroyHash);
    new (hash) Hash();
    return hash;
}

void _ClearHashKey(Hash* hash, const char* key) {
    if (Contains(hash, key) && (*hash)[key].type == TYPE_HASH) {
        _DecRef((*hash)[key].value.t);
    }
}

Hash* _SetHashInt(Hash* hash, const char* key, int value) {
    _ClearHashKey(hash, key);
    return hash;
}

Hash* _SetHashReal(Hash* hash, const char* key, float value) {
    _ClearHashKey(hash, key);
    (*hash)[key] = ValueFromReal(value);
    return hash;
}

Hash* _SetHashString(Hash* hash, const char* key, const char* value) {
    _ClearHashKey(hash, key);
    (*hash)[key] = ValueFromString(value);
    return hash;
}

Hash* _SetHashHash(Hash* hash, const char* key, Hash* value) {
    _IncRef(value);
    _ClearHashKey(hash, key);
    (*hash)[key] = ValueFromHash(value);
    _DecRef(value);
    return hash;
}

Hash* _SetHashRef(Hash* hash, const char* key, void* value) {
    _ClearHashKey(hash, key);
    (*hash)[key] = ValueFromRef(value);
    return hash;
}

int _HashInt(const Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToInt((*(Hash*)hash)[key])
        : 0;
}

float _HashReal(const Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToReal((*(Hash*)hash)[key])
        : 0.0f;
}

const char* _HashString(const Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToString((*(Hash*)hash)[key])
        : "";
}

Hash* _HashHash(const Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToHash((*(Hash*)hash)[key])
        : NULL;
}

void* _HashRef(const Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToRef((*(Hash*)hash)[key])
        : NULL;
}

int Contains(const Hash* hash, const char* key) {
    return hash->count(key) > 0;
}

void Remove(Hash* hash, const char* key) {
    if (Contains(hash, key)) {
        hash->erase(key);
    }
}

int Size(const Hash* hash) {
    return hash->size();
}

void Clear(Hash* hash) {
    return hash->clear();
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
