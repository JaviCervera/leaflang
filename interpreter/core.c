#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#ifndef _MSC_VER
#include <dirent.h>
#include <unistd.h>
#define _getcwd getcwd
#define _chdir chdir
#else
#include <direct.h>
#include "dirent.h"
#endif
#include "../src/types.h"
#define LITE_MEM_IMPLEMENTATION
#include "litemem.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

#if defined _WIN32 && !defined S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFDIR) == _S_IFDIR)
#endif

typedef struct Memory {
    char* ptr;
    size_t size;
} Memory;

#define CORE_IMPL
#include "core.h"

// ------------------------------------
// App
// ------------------------------------

static char* pico_appName = NULL;
static struct Hash* pico_appArgs = NULL;

void _SetArgs(int argc, const char* argv[]) {
    pico_appName = lstr_alloc(argv[0]);
    pico_appArgs = (struct Hash*)_IncRef(_CreateHash());
    for (int i = 1; i < argc; ++i) {
        _SetHashString(pico_appArgs, Str(i - 1), argv[i]);
    }
}

const char* AppName() {
    return pico_appName;
}

struct Hash* AppArgs() {
    return pico_appArgs;
}

const char* Run(const char* command) {
    char tmp[65536];
    tmp[0] = '\0';
    FILE* pipe = popen(command, "rt");
    if (!pipe) return lstr_get("");
    while (!feof(pipe)) {
        char tmp2[128];
        if (fgets(tmp2, 128, pipe) != 0) strcat(tmp, tmp2);
    }
    pclose(pipe);
    return lstr_get(tmp);
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
    char buffer[1024];
    printf("%s", prompt);
    fgets(buffer, 1024, stdin);
    return lstr_get(buffer);
}

void Print(const char* msg) {
    printf("%s\n", msg);
    fflush(stdout);
}

// ------------------------------------
// Dir
// ------------------------------------

struct Hash* DirContents(const char* path) {
    struct Hash* hash = _CreateHash();
    DIR* d = (DIR*)opendir(path);
    if (d == NULL) return hash;
    struct dirent* entry;
    int i = 0;
    while ((entry = (struct dirent*)readdir(d))) {
        _SetHashString(hash, Str(i++), entry->d_name);
    }
    closedir(d);
    return hash;
}

const char* CurrentDir() {
    char buf[FILENAME_MAX];
    _getcwd(buf, FILENAME_MAX);
    return lstr_get(buf);
}

void ChangeDir(const char* dir) {
    _chdir(dir);
}

const char* FullPath(const char* filename) {
    char out_path[FILENAME_MAX];
    realpath(filename, out_path);
    return lstr_get(out_path);
}

// ------------------------------------
// File
// ------------------------------------

int FileType(const char* filename) {
    struct stat statbuf;
    if (stat(filename, &statbuf) == -1) return 0;
    else if (S_ISDIR(statbuf.st_mode)) return 2;
    else return 1;
}

void DeleteFile(const char* filename) {
    remove(filename);
}

// ------------------------------------
// Hash
// ------------------------------------

typedef struct {
    int type;
    union {
        int i;
        float f;
        char* s;
        struct Hash* h;
        void* r;
    } value;
} Value;

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
    lmem_assign(v.value.s, lstr_get(s));
    return v;
}

Value ValueFromHash(struct Hash* h) {
    Value v = {0};
    v.type = TYPE_HASH;
    lmem_assign(v.value.h, h);
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
    case TYPE_HASH: return _HashToString(v.value.h);
    default: return lstr_get("");
    }
}

struct Hash* ValueToHash(const Value v) {
    switch (v.type) {
    case TYPE_INT: return NULL;
    case TYPE_REAL: return NULL;
    case TYPE_STRING: return NULL;
    case TYPE_REF: return (struct Hash*)v.value.r;
    default: return v.value.h;
    }
}

void* ValueToRef(const Value v) {
    switch (v.type) {
    case TYPE_INT: return NULL;
    case TYPE_REAL: return NULL;
    case TYPE_STRING: return v.value.s;
    case TYPE_HASH: return v.value.h;
    default: return v.value.r;
    }
}

int ValueIsManaged(const Value v) {
    return v.type == TYPE_STRING || v.type == TYPE_HASH;
}

typedef struct {
    const char* key;
    Value value;
} HashEntry;

typedef struct Hash {
    HashEntry* entries;
} Hash;

void _ClearHashValue(Hash* hash, const char* key) {
    const Value value = shget(hash->entries, key);
    if (Contains(hash, key) && ValueIsManaged(value)) {
        _DecRef(value.value.r);
    }
}

void _DestroyHash(Hash* hash) {
    for (size_t i = 0; i < shlenu(hash->entries); ++i) {
        if (ValueIsManaged(hash->entries[i].value)) {
            _DecRef(hash->entries[i].value.value.r);
        }
    }
    shfree(hash->entries);
}

Hash* _CreateHash() {
    Hash* hash = lmem_allocauto(Hash, (void*)_DestroyHash);
    hash->entries = NULL;
    return hash;
}

Hash* _SetHashInt(Hash* hash, const char* key, int value) {
    _ClearHashValue(hash, key);
    shput(hash->entries, key, ValueFromInt(value));
    return hash;
}

Hash* _SetHashReal(Hash* hash, const char* key, float value) {
    _ClearHashValue(hash, key);
    shput(hash->entries, key, ValueFromReal(value));
    return hash;
}

Hash* _SetHashString(Hash* hash, const char* key, const char* value) {
    _IncRef((char*)value);
    _ClearHashValue(hash, key);
    shput(hash->entries, key, ValueFromString(value));
    _DecRef((char*)value);
    return hash;
}

Hash* _SetHashHash(Hash* hash, const char* key, Hash* value) {
    _IncRef(value);
    _ClearHashValue(hash, key);
    shput(hash->entries, key, ValueFromHash(value));
    _DecRef(value);
    return hash;
}

Hash* _SetHashRef(Hash* hash, const char* key, void* value) {
    _ClearHashValue(hash, key);
    shput(hash->entries, key, ValueFromRef(value));
    return hash;
}

int _HashInt(Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToInt(shget(hash->entries, key))
        : 0;
}

float _HashReal(Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToReal(shget(hash->entries, key))
        : 0.0f;
}

const char* _HashString(Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToString(shget(hash->entries, key))
        : "";
}

Hash* _HashHash(Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToHash(shget(hash->entries, key))
        : NULL;
}

void* _HashRef(Hash* hash, const char* key) {
    return (Contains(hash, key))
        ? ValueToRef(shget(hash->entries, key))
        : NULL;
}

const char* _HashToString(Hash* hash) {
    char content[65536];
    content[0] = '\0';
    strcpy(content, "{");
    for (size_t i = 0; i < shlenu(hash->entries); ++i) {
        const HashEntry* entry = &hash->entries[i];
        const char* prefix = (entry->value.type == TYPE_STRING)
            ? "\""
            : "";
        if (i > 0) strcat(content, ", ");
        strcat(content, "\"");
        strcat(content, entry->key);
        strcat(content, "\": ");
        strcat(content, prefix);
        strcat(content, ValueToString(entry->value));
        strcat(content, prefix);
    }
    strcat(content, "}");
    return lstr_get(content);
}

int Contains(Hash* hash, const char* key) {
    return shlenu(hash->entries) > 0;
}

void Remove(Hash* hash, const char* key) {
    if (Contains(hash, key)) {
        shdel(hash->entries, key);
    }
}

int Size(Hash* hash) {
    return shlenu(hash->entries);
}

void Clear(Hash* hash) {
    shfree(hash->entries);
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
    return fabsf(x);
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
    return (x >= y) ? x : y;
}

float Min(float x, float y) {
    return (x <= y) ? x : y;
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
    return (int)num;
}

// ------------------------------------
// Memory
// ------------------------------------

Memory* Dim(int size) {
    Memory* mem = (Memory*)malloc(sizeof(Memory));
    mem->ptr = (char*)calloc(1, size);
    mem->size = size;
    return mem;
}

void Undim(Memory* mem) {
    free(mem->ptr);
    free(mem);
}

void Redim(Memory* mem, int size) {
    mem->ptr = (char*)realloc(mem->ptr, size);
    mem->size = size;
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
    char result[65536];
    result[0] = '\0';
    int c;
    while (offset < DimSize(mem) && (c = PeekByte(mem, offset)) != 0) {
        char s[] = {c, '\0'};
        strcat(result, s);
        ++offset;
    }
    return lstr_get(result);
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
    return strlen(str);
}

const char* Left(const char* str, int count) {
    char* result = lstr_allocempty(count);
    strncpy(result, str, count);
    return (const char*)lmem_autorelease(result);
}

const char* Right(const char* str, int count) {
    char* result = lstr_allocempty(count);
    strncpy(result, str + strlen(str) - count, count);
    return (const char*)lmem_autorelease(result);
}

const char* Mid(const char* str, int offset, int count) {
    char* result = lstr_allocempty(count);
    strncpy(result, str + offset, count);
    return (const char*)lmem_autorelease(result);
}

const char* Lower(const char* str) {
    const size_t len = strlen(str);
    char* result = lstr_allocempty(len);
    for (size_t i = 0; i < len; ++i) {
        result[i] = (char)tolower(str[i]);
    }
    return (const char*)lmem_autorelease(result);
}

const char* Upper(const char* str) {
    const size_t len = strlen(str);
    char* result = lstr_allocempty(len);
    for (size_t i = 0; i < len; ++i) {
        result[i] = (char)toupper(str[i]);
    }
    return (const char*)lmem_autorelease(result);
}

int Find(const char* str, const char* find, int offset) {
    const char* p = strstr(&str[offset], find);
    if (p == NULL)
        return -1;
    else
        return (p - str);
}

char* _ReplaceOne(const char* str, size_t pos, size_t len, const char* rep, size_t rlen) {
    char* result = lstr_allocempty(strlen(str) + rlen - len);
    strncpy(result, str, pos);
    strcat(result, rep);
    strcat(result, &str[pos + len]);
    return (char*)lmem_autorelease(result);
}

const char* Replace(const char* str, const char* find, const char* replace) {
    char* result = lstr_get(str);
    const size_t rlen = strlen(replace);
    const size_t find_len = strlen(find);
    size_t find_pos = Find(result, find, 0);
    while (find_pos != -1) {
        result = _ReplaceOne(result, find_pos, find_len, replace, rlen);
        find_pos = Find(result, find, find_pos + rlen);
    }
    return result;
}

const char* Trim(const char* str) {
    const size_t len = strlen(str);
    size_t offset = 0;
    while (offset < len && isspace(str[offset])) ++offset;
    size_t count = len - offset - 1;
    while (count > 0 && isspace(str[offset + count])) --count;
    return Mid(str, offset, count + 1);
}

const char* Join(Hash* hash, const char* separator) {
    size_t current_len = 0;
    size_t current_max = 1000;
    char* tmp = (char*)calloc(current_max, sizeof(char));
    const int size = Size(hash);
    const size_t seplen = strlen(separator);
    for (int i = 0; i < size; ++i) {
        const char* str = (const char*)_IncRef((void*)_HashString(hash, Str(i)));
        size_t len = strlen(str);
        if (i > 0) len += seplen;
        if (current_max < current_len + len + 1) {
            current_max += (int)Max(1000, len + 1);
            tmp = (char*)realloc(tmp, current_max * sizeof(char));
        }
        if (i > 0) strcat(tmp, separator);
        strcat(tmp, str);
        current_len += len;
        _DecRef((void*)str);
    }
    char* result = lstr_get(tmp);
    free(tmp);
    return result;
}

Hash* _SplitChars(const char* str) {
    const int len = Len(str);
    Hash* hash = _CreateHash();
    for (int i = 0; i < len; ++i) {
        _SetHashString(hash, Str(i), Chr(i));
    }
    return hash;
}

Hash* _SplitBySep(const char* str, const char* separator) {
    const size_t seplen = strlen(separator);
    Hash* hash = _CreateHash();
    int prevoffset = 0;
    int nextoffset = 0;
    int i = 0;
    while ((nextoffset = Find(str, separator, prevoffset)) != -1) {
        _SetHashString(hash, Str(i++), Mid(str, prevoffset, nextoffset - prevoffset));
        prevoffset = nextoffset + seplen;
    }
    _SetHashString(hash, Str(i++), str + prevoffset);
    return hash;
}

Hash* Split(const char* str, const char* separator) {
    if (strcmp(separator, "") == 0) {
        return _SplitChars(str);
    } else {
        return _SplitBySep(str, separator);
    }
}

const char* StripExt(const char* filename) {
    const char* endp = strrchr(filename, '.');
    if (!endp) return lstr_get(filename);
    return Mid(filename, 0, endp - filename);
}

const char* StripDir(const char* filename) {
    const char* fendp = strrchr(filename, '/');
    const char* bendp = strrchr(filename, '\\');
    const char* endp = (fendp >= bendp) ? fendp : bendp;
    if (!endp) return lstr_get(filename);
    return Mid(filename, 0, endp - filename);
}

const char* ExtractExt(const char* filename) {
    const char* endp = strrchr(filename, '.');
    if (!endp) return lstr_get("");
    const size_t offset = endp - filename + 1;
    return Mid(filename, offset, strlen(filename) - offset);
}

const char* ExtractDir(const char* filename) {
    const char* fendp = strrchr(filename, '/');
    const char* bendp = strrchr(filename, '\\');
    const char* endp = (fendp >= bendp) ? fendp : bendp;
    if (!endp) return lstr_get("");
    const size_t size = endp - filename;
    return Mid(filename, 0, size);
}

int Asc(const char* str, int index) {
    return (int)str[index];
}

const char* Chr(int c) {
    const char str[] = {c, '\0'};
    return lstr_get(str);
}

const char* Str(int val) {
    char str[64];
    sprintf(str, "%i", val);
    return lstr_get(str);
}

const char* StrF(float val) {
    char str[64];
    sprintf(str, "%f", val);
    return lstr_get(str);
}

int Val(const char* str) {
    int val = 0;
    sscanf(str, "%i", &val);
    return val;
}

float ValF(const char* str) {
    float val = 0;
    sscanf(str, "%f", &val);
    return val;
}

const char* LoadString(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return lstr_get("");
    fseek(f, 0, SEEK_END);
    const long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = (char*)malloc(size+1);
    fread(buf, sizeof(char), size, f);
    buf[size] = '\0';
    const char* result = lstr_get(buf);
    free(buf);
    return result;
}

void SaveString(const char* filename, const char* str, int append) {
    FILE* f = fopen(filename, append ? "ab" : "wb");
    if (!f) return;
    fwrite(str, sizeof(char), strlen(str), f);
    fclose(f);
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
    return lstr_get("");
}

int Callable(const char* name) {
    return 0;
}