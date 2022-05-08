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
#define LITE_MEM_IMPLEMENTATION
#include "litemem.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define realpath(N,R) _fullpath((R),(N),_MAX_PATH)
#if !defined S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFDIR) == _S_IFDIR)
#endif
#endif

typedef struct TMemory {
    char* ptr;
    size_t size;
} TMemory;

#define CORE_IMPL
#include "core.h"

// ------------------------------------
// App
// ------------------------------------

static TChar* leaf_appName = NULL;
static struct TList* leaf_appArgs = NULL;

void _SetArgs(int argc, char* argv[]) {
    leaf_appName = lstr_alloc(argv[0]);
    leaf_appArgs = (struct TList*)_IncRef(_CreateList());
    for (TInt i = 1; i < argc; ++i) {
        _SetListString(leaf_appArgs, i - 1, argv[i]);
    }
}

const TChar* AppName() {
    return leaf_appName;
}

struct TList* AppArgs() {
    return leaf_appArgs;
}

const TChar* Run(const TChar* command) {
    TChar tmp[65536];
    tmp[0] = '\0';
    FILE* pipe = popen(command, "r");
    if (!pipe) return lstr_get("");
    while (!feof(pipe)) {
        TChar tmp2[65536];
        if (fgets(tmp2, 65536, pipe) != 0) strcat(tmp, tmp2);
    }
    pclose(pipe);
    return lstr_get(tmp);
}

TInt System(const TChar* command) {
    return system(command);
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

const TChar* Input(const TChar* prompt) {
    TChar buffer[1024];
    printf("%s", prompt);
    fgets(buffer, 1024, stdin);
    return lstr_get(buffer);
}

void Print(const TChar* msg) {
    printf("%s\n", msg);
    fflush(stdout);
}

// ------------------------------------
// Dir
// ------------------------------------

struct TList* DirContents(const TChar* path) {
    struct TList* list = _CreateList();
    DIR* d = (DIR*)opendir(path);
    if (d == NULL) return list;
    struct dirent* entry;
    TInt i = 0;
    while ((entry = (struct dirent*)readdir(d))) {
        _SetListString(list, i++, entry->d_name);
    }
    closedir(d);
    return list;
}

const TChar* CurrentDir() {
    TChar buf[FILENAME_MAX];
    _getcwd(buf, FILENAME_MAX);
    return lstr_get(buf);
}

void ChangeDir(const TChar* dir) {
    _chdir(dir);
}

const TChar* FullPath(const TChar* filename) {
    TChar out_path[FILENAME_MAX];
    realpath(filename, out_path);
    return lstr_get(out_path);
}

// ------------------------------------
// File
// ------------------------------------

TInt FileType(const TChar* filename) {
    struct stat statbuf;
    if (stat(filename, &statbuf) == -1) return 0;
    else if (S_ISDIR(statbuf.st_mode)) return 2;
    else return 1;
}

void DeleteFile(const TChar* filename) {
    remove(filename);
}

// ------------------------------------
// List
// ------------------------------------

typedef struct {
    TInt type;
    union {
        TInt i;
        TFloat f;
        TChar* s;
        struct TList* l;
        struct TDict* h;
        void* r;
    } value;
} Value;

Value ValueFromInt(TInt i) {
    Value v = {0};
    v.type = TYPE_INT;
    v.value.i = i;
    return v;
}

Value ValueFromFloat(TFloat f) {
    Value v = {0};
    v.type = TYPE_FLOAT;
    v.value.f = f;
    return v;
}

Value ValueFromString(const TChar* s) {
    Value v = {0};
    v.type = TYPE_STRING;
    lmem_assign(v.value.s, lstr_get(s));
    return v;
}

Value ValueFromList(struct TList* l) {
    Value v = {0};
    v.type = TYPE_LIST;
    lmem_assign(v.value.l, l);
    return v;
}

Value ValueFromDict(struct TDict* h) {
    Value v = {0};
    v.type = TYPE_DICT;
    lmem_assign(v.value.h, h);
    return v;
}

Value ValueFromRaw(void* r) {
    Value v = {0};
    v.type = TYPE_RAW;
    v.value.r = r;
    return v;
}

TInt ValueToInt(const Value v) {
    switch (v.type) {
    case TYPE_INT: return v.value.i;
    case TYPE_FLOAT: return (TInt)v.value.f;
    case TYPE_STRING: return Val(v.value.s);
    default: return 0;
    }
}

TFloat ValueToFloat(const Value v) {
    switch (v.type) {
    case TYPE_INT: return v.value.i;
    case TYPE_FLOAT: return v.value.f;
    case TYPE_STRING: return ValF(v.value.s);
    default: return 0.0f;
    }
}

const TChar* ValueToString(const Value v) {
    switch (v.type) {
    case TYPE_INT: return Str(v.value.i);
    case TYPE_FLOAT: return StrF(v.value.f);
    case TYPE_STRING: return v.value.s;
    case TYPE_LIST: return _ListToString(v.value.l);
    case TYPE_DICT: return _DictToString(v.value.h);
    default: return lstr_get("");
    }
}

struct TList* ValueToList(const Value v) {
    switch (v.type) {
    case TYPE_RAW: return (struct TList*)v.value.r;
    case TYPE_LIST: return v.value.l;
    default: return _CreateList();
    }
}

struct TDict* ValueToDict(const Value v) {
    switch (v.type) {
    case TYPE_RAW: return (struct TDict*)v.value.r;
    case TYPE_DICT: return v.value.h;
    default: return _CreateDict();
    }
}

void* ValueToRaw(const Value v) {
    switch (v.type) {
    case TYPE_INT: return NULL;
    case TYPE_FLOAT: return NULL;
    case TYPE_STRING: return v.value.s;
    case TYPE_LIST: return v.value.l;
    case TYPE_DICT: return v.value.h;
    default: return v.value.r;
    }
}

TInt ValueIsManaged(const Value v) {
    return v.type == TYPE_STRING || v.type == TYPE_LIST || v.type == TYPE_DICT;
}

typedef struct TList {
    Value* elems;
} TList;

void _ClearListValue(TList* list, size_t index) {
    if (index >= 0 && index < ListSize(list)) {
        const Value value = list->elems[index];
        if (ValueIsManaged(value)) {
            _DecRef(value.value.r);
        }
    }
}

void _DestroyList(TList* list) {
    for (size_t i = 0; i < arrlenu(list->elems); ++i) {
        _ClearListValue(list, i);
    }
    arrfree(list->elems);
    list->elems = NULL;
}

TList* _CreateList() {
    TList* list = lmem_allocauto(TList, (void*)_DestroyList);
    list->elems = NULL;
    return list;
}

TList* _SetListInt(TList* list, size_t index, TInt value) {
    _ClearListValue(list, index);
    if (index >= ListSize(list)) arrsetlen(list->elems, index + 1);
    list->elems[index] = ValueFromInt(value);
    return list;
}

TList* _SetListFloat(TList* list, size_t index, TFloat value) {
    _ClearListValue(list, index);
    if (index >= ListSize(list)) arrsetlen(list->elems, index + 1);
    list->elems[index] = ValueFromFloat(value);
    return list;
}

TList* _SetListString(TList* list, size_t index, const TChar* value) {
    _IncRef((TChar*)value);
    _ClearListValue(list, index);
    if (index >= ListSize(list)) arrsetlen(list->elems, index + 1);
    list->elems[index] = ValueFromString(value);
    _DecRef((TChar*)value);
    return list;
}

TList* _SetListList(TList* list, size_t index, TList* value) {
    _IncRef((TChar*)value);
    _ClearListValue(list, index);
    if (index >= ListSize(list)) arrsetlen(list->elems, index + 1);
    list->elems[index] = ValueFromList(value);
    _DecRef((TChar*)value);
    return list;
}

TList* _SetListDict(TList* list, size_t index, struct TDict* value) {
    _IncRef((TChar*)value);
    _ClearListValue(list, index);
    if (index >= ListSize(list)) arrsetlen(list->elems, index + 1);
    list->elems[index] = ValueFromDict(value);
    _DecRef((TChar*)value);
    return list;
}

TList* _SetListRaw(TList* list, size_t index, void* value) {
    _ClearListValue(list, index);
    if (index >= ListSize(list)) arrsetlen(list->elems, index + 1);
    list->elems[index] = ValueFromRaw(value);
    return list;
}

TInt _ListInt(TList* list, size_t index) {
    return (index >= 0 && index < ListSize(list))
        ? ValueToInt(list->elems[index])
        : 0;
}

TFloat _ListFloat(TList* list, size_t index) {
    return (index >= 0 && index < ListSize(list))
        ? ValueToFloat(list->elems[index])
        : 0.0f;
}

const TChar* _ListString(TList* list, size_t index) {
    return (index >= 0 && index < ListSize(list))
        ? ValueToString(list->elems[index])
        : lstr_get("");
}

TList* _ListList(TList* list, size_t index) {
    return (index >= 0 && index < ListSize(list))
        ? ValueToList(list->elems[index])
        : _CreateList();
}

struct TDict* _ListDict(TList* list, size_t index) {
    return (index >= 0 && index < ListSize(list))
        ? ValueToDict(list->elems[index])
        : _CreateDict();
}

void* _ListRaw(TList* list, size_t index) {
    return (index >= 0 && index < ListSize(list))
        ? ValueToRaw(list->elems[index])
        : NULL;
}

const TChar* _ListToString(TList* list) {
    TChar content[65536];
    content[0] = '\0';
    strcpy(content, "[");
    for (size_t i = 0; i < arrlenu(list->elems); ++i) {
        const Value value = list->elems[i];
        const TChar* prefix = (value.type == TYPE_STRING)
            ? "\""
            : "";
        if (i > 0) strcat(content, ", ");
        strcat(content, prefix);
        strcat(content, ValueToString(value));
        strcat(content, prefix);
    }
    strcat(content, "]");
    return lstr_get(content);
}

void RemoveIndex(TList* list, TInt index) {
    if (index >= 0 && index < ListSize(list)) {
        _ClearListValue(list, index);
        arrdel(list->elems, index);
    }
}

TInt ListSize(TList* list) {
    return arrlenu(list->elems);
}

void ClearList(TList* list) {
    _DestroyList(list);
}

// ------------------------------------
// Dict
// ------------------------------------

typedef struct {
    const TChar* key;
    Value value;
} DictEntry;

typedef struct TDict {
    DictEntry* entries;
} TDict;

void _ClearDictValue(TDict* dict, const TChar* key) {
    const Value value = shget(dict->entries, key);
    if (Contains(dict, key) && ValueIsManaged(value)) {
        _DecRef(value.value.r);
    }
}

void _DestroyDict(TDict* dict) {
    for (size_t i = 0; i < shlenu(dict->entries); ++i) {
        if (ValueIsManaged(dict->entries[i].value)) {
            _DecRef(dict->entries[i].value.value.r);
        }
    }
    shfree(dict->entries);
    dict->entries = NULL;
}

TDict* _CreateDict() {
    TDict* dict = lmem_allocauto(TDict, (void*)_DestroyDict);
    dict->entries = NULL;
    return dict;
}

TDict* _SetDictInt(TDict* dict, const TChar* key, TInt value) {
    _ClearDictValue(dict, key);
    shput(dict->entries, key, ValueFromInt(value));
    return dict;
}

TDict* _SetDictFloat(TDict* dict, const TChar* key, TFloat value) {
    _ClearDictValue(dict, key);
    shput(dict->entries, key, ValueFromFloat(value));
    return dict;
}

TDict* _SetDictString(TDict* dict, const TChar* key, const TChar* value) {
    _IncRef((TChar*)value);
    _ClearDictValue(dict, key);
    shput(dict->entries, key, ValueFromString(value));
    _DecRef((TChar*)value);
    return dict;
}

TDict* _SetDictList(TDict* dict, const TChar* key, TList* value) {
    _IncRef(value);
    _ClearDictValue(dict, key);
    shput(dict->entries, key, ValueFromList(value));
    _DecRef(value);
    return dict;
}

TDict* _SetDictDict(TDict* dict, const TChar* key, TDict* value) {
    _IncRef(value);
    _ClearDictValue(dict, key);
    shput(dict->entries, key, ValueFromDict(value));
    _DecRef(value);
    return dict;
}

TDict* _SetDictRaw(TDict* dict, const TChar* key, void* value) {
    _ClearDictValue(dict, key);
    shput(dict->entries, key, ValueFromRaw(value));
    return dict;
}

TInt _DictInt(TDict* dict, const TChar* key) {
    return (Contains(dict, key))
        ? ValueToInt(shget(dict->entries, key))
        : 0;
}

TFloat _DictFloat(TDict* dict, const TChar* key) {
    return (Contains(dict, key))
        ? ValueToFloat(shget(dict->entries, key))
        : 0.0f;
}

const TChar* _DictString(TDict* dict, const TChar* key) {
    return (Contains(dict, key))
        ? ValueToString(shget(dict->entries, key))
        : lstr_get("");
}

TList* _DictList(TDict* dict, const TChar* key) {
    return (Contains(dict, key))
        ? ValueToList(shget(dict->entries, key))
        : _CreateList();
}

TDict* _DictDict(TDict* dict, const TChar* key) {
    return (Contains(dict, key))
        ? ValueToDict(shget(dict->entries, key))
        : _CreateDict();
}

void* _DictRaw(TDict* dict, const TChar* key) {
    return (Contains(dict, key))
        ? ValueToRaw(shget(dict->entries, key))
        : NULL;
}

const TChar* _DictToString(TDict* dict) {
    TChar content[65536];
    content[0] = '\0';
    strcpy(content, "{");
    for (size_t i = 0; i < shlenu(dict->entries); ++i) {
        const DictEntry* entry = &dict->entries[i];
        const TChar* prefix = (entry->value.type == TYPE_STRING)
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

TInt Contains(TDict* dict, const TChar* key) {
    return shlenu(dict->entries) > 0;
}

void RemoveKey(TDict* dict, const TChar* key) {
    if (Contains(dict, key)) {
        _ClearDictValue(dict, key);
        shdel(dict->entries, key);
    }
}

TInt DictSize(TDict* dict) {
    return shlenu(dict->entries);
}

void ClearDict(TDict* dict) {
    _DestroyDict(dict);
}

// ------------------------------------
// Math
// ------------------------------------

TFloat ASin(TFloat x) {
    return asin(x);
}

TFloat ATan(TFloat x) {
    return atan(x);
}

TFloat ATan2(TFloat x, TFloat y) {
    return atan2(x, y);
}

TFloat Abs(TFloat x) {
    return fabsf(x);
}

TFloat Ceil(TFloat x) {
    return ceil(x);
}

TFloat Clamp(TFloat x, TFloat min, TFloat max) {
    return Min(Max(x, min), max);
}

TFloat Cos(TFloat x) {
    return cos(x);
}

TFloat Exp(TFloat x) {
    return exp(x);
}

TFloat Floor(TFloat x) {
    return floor(x);
}

TFloat Log(TFloat x) {
    return log(x);
}

TFloat Max(TFloat x, TFloat y) {
    return (x >= y) ? x : y;
}

TFloat Min(TFloat x, TFloat y) {
    return (x <= y) ? x : y;
}

TFloat Pow(TFloat x, TFloat y) {
    return pow(x, y);
}

TFloat Sgn(TFloat x) {
    return (0 < x) - (x < 0);
}

TFloat Sin(TFloat x) {
    return sin(x);
}

TFloat Sqrt(TFloat x) {
    return sqrt(x);
}

TFloat Tan(TFloat x) {
    return tan(x);
}

TInt Int(TFloat num) {
    return (TInt)num;
}

// ------------------------------------
// TMemory
// ------------------------------------

TMemory* Dim(TInt size) {
    TMemory* mem = (TMemory*)malloc(sizeof(TMemory));
    mem->ptr = (char*)calloc(1, size);
    mem->size = size;
    return mem;
}

void Undim(TMemory* mem) {
    free(mem->ptr);
    free(mem);
}

void Redim(TMemory* mem, TInt size) {
    mem->ptr = (char*)realloc(mem->ptr, size);
    mem->size = size;
}

TMemory* LoadDim(const TChar* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    size_t size = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    TMemory* mem = Dim(size);
    fread(mem->ptr, size, 1, f);
    fclose(f);
    return mem;
}

void SaveDim(TMemory* mem, const TChar* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    fwrite(mem->ptr, DimSize(mem), 1, f);
    fclose(f);
}

TInt DimSize(TMemory* mem) {
    return (TInt)mem->size;
}

TInt PeekByte(TMemory* mem, TInt offset) {
    unsigned char v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return (TInt)v;
}

TInt PeekShort(TMemory* mem, TInt offset) {
    unsigned short v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return (TInt)v;
}

TInt PeekInt(TMemory* mem, TInt offset) {
    TInt v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return v;
}

TFloat PeekFloat(TMemory* mem, TInt offset) {
    TFloat v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return v;
}

const TChar* PeekString(TMemory* mem, TInt offset) {
    TChar result[65536];
    result[0] = '\0';
    TInt c;
    while (offset < DimSize(mem) && (c = PeekByte(mem, offset)) != 0) {
        TChar s[] = {(TChar)c, '\0'};
        strcat(result, s);
        ++offset;
    }
    return lstr_get(result);
}

void* PeekRaw(TMemory* mem, TInt offset) {
    void* v;
    memcpy(&v, &mem->ptr[offset], sizeof(v));
    return v;
}

void PokeByte(TMemory* mem, TInt offset, TInt val) {
    unsigned char* b = (unsigned char*)&val;
    memcpy(&(mem->ptr[offset]), &b[3], sizeof(unsigned char));
}

void PokeShort(TMemory* mem, TInt offset, TInt val) {
    unsigned short* s = (unsigned short*)&val;
    memcpy(&(mem->ptr[offset]), &s[1], sizeof(unsigned short));
}

void PokeInt(TMemory* mem, TInt offset, TInt val) {
    memcpy(&(mem->ptr[offset]), &val, sizeof(val));
}

void PokeFloat(TMemory* mem, TInt offset, TFloat val) {
    memcpy(&(mem->ptr[offset]), &val, sizeof(val));
}

void PokeString(TMemory* mem, TInt offset, const TChar* val) {
    memcpy(&(mem->ptr[offset]), val, strlen(val) + 1);
}

void PokeRaw(TMemory* mem, TInt offset, void* val) {
    memcpy(&(mem->ptr[offset]), &val, sizeof(val));
}

// ------------------------------------
// String
// ------------------------------------

TInt Len(const TChar* str) {
    return strlen(str);
}

const TChar* Left(const TChar* str, TInt count) {
    TChar* result = lstr_allocempty(count);
    strncpy(result, str, count);
    return (const TChar*)lmem_autorelease(result);
}

const TChar* Right(const TChar* str, TInt count) {
    TChar* result = lstr_allocempty(count);
    strncpy(result, str + strlen(str) - count, count);
    return (const TChar*)lmem_autorelease(result);
}

const TChar* Mid(const TChar* str, TInt offset, TInt count) {
    TChar* result = lstr_allocempty(count);
    strncpy(result, str + offset, count);
    return (const TChar*)lmem_autorelease(result);
}

const TChar* Lower(const TChar* str) {
    const size_t len = strlen(str);
    TChar* result = lstr_allocempty(len);
    for (size_t i = 0; i < len; ++i) {
        result[i] = (TChar)tolower(str[i]);
    }
    return (const TChar*)lmem_autorelease(result);
}

const TChar* Upper(const TChar* str) {
    const size_t len = strlen(str);
    TChar* result = lstr_allocempty(len);
    for (size_t i = 0; i < len; ++i) {
        result[i] = (TChar)toupper(str[i]);
    }
    return (const TChar*)lmem_autorelease(result);
}

TInt Find(const TChar* str, const TChar* find, TInt offset) {
    const TChar* p = strstr(&str[offset], find);
    if (p == NULL)
        return -1;
    else
        return (p - str);
}

TChar* _ReplaceOne(const TChar* str, size_t pos, size_t len, const TChar* rep, size_t rlen) {
    TChar* result = lstr_allocempty(strlen(str) + rlen - len);
    strncpy(result, str, pos);
    strcat(result, rep);
    strcat(result, &str[pos + len]);
    return (TChar*)lmem_autorelease(result);
}

const TChar* Replace(const TChar* str, const TChar* find, const TChar* replace) {
    TChar* result = lstr_get(str);
    const size_t rlen = strlen(replace);
    const size_t find_len = strlen(find);
    size_t find_pos = Find(result, find, 0);
    while (find_pos != -1) {
        result = _ReplaceOne(result, find_pos, find_len, replace, rlen);
        find_pos = Find(result, find, find_pos + rlen);
    }
    return result;
}

const TChar* Trim(const TChar* str) {
    const size_t len = strlen(str);
    size_t offset = 0;
    while (offset < len && isspace(str[offset])) ++offset;
    size_t count = len - offset - 1;
    while (count > 0 && isspace(str[offset + count])) --count;
    return Mid(str, offset, count + 1);
}

const TChar* Join(TList* list, const TChar* separator) {
    size_t current_len = 0;
    size_t current_max = 1000;
    TChar* tmp = (TChar*)calloc(current_max, sizeof(TChar));
    const TInt size = ListSize(list);
    const size_t seplen = strlen(separator);
    for (TInt i = 0; i < size; ++i) {
        const TChar* str = (const TChar*)_IncRef((void*)_ListString(list, i));
        size_t len = strlen(str);
        if (i > 0) len += seplen;
        if (current_max < current_len + len + 1) {
            current_max += (TInt)Max(1000, len + 1);
            tmp = (TChar*)realloc(tmp, current_max * sizeof(TChar));
        }
        if (i > 0) strcat(tmp, separator);
        strcat(tmp, str);
        current_len += len;
        _DecRef((void*)str);
    }
    TChar* result = lstr_get(tmp);
    free(tmp);
    return result;
}

TList* _SplitChars(const TChar* str) {
    const TInt len = Len(str);
    TList* list = _CreateList();
    for (TInt i = 0; i < len; ++i) {
        _SetListString(list, i, Chr(str[i]));
    }
    return list;
}

TList* _SplitBySep(const TChar* str, const TChar* separator) {
    const size_t seplen = strlen(separator);
    TList* list = _CreateList();
    TInt prevoffset = 0;
    TInt nextoffset = 0;
    TInt i = 0;
    while ((nextoffset = Find(str, separator, prevoffset)) != -1) {
        _SetListString(list, i++, Mid(str, prevoffset, nextoffset - prevoffset));
        prevoffset = nextoffset + seplen;
    }
    _SetListString(list, i++, lstr_get(str + prevoffset));
    return list;
}

TList* Split(const TChar* str, const TChar* separator) {
    if (strcmp(separator, "") == 0) {
        return _SplitChars(str);
    } else {
        return _SplitBySep(str, separator);
    }
}

const TChar* StripExt(const TChar* filename) {
    const TChar* endp = strrchr(filename, '.');
    if (!endp) return lstr_get(filename);
    return Mid(filename, 0, endp - filename);
}

const TChar* StripDir(const TChar* filename) {
    const TChar* fendp = strrchr(filename, '/');
    const TChar* bendp = strrchr(filename, '\\');
    const TChar* endp = (fendp >= bendp) ? fendp : bendp;
    if (!endp) return lstr_get(filename);
    return Mid(filename, 0, endp - filename);
}

const TChar* ExtractExt(const TChar* filename) {
    const TChar* endp = strrchr(filename, '.');
    if (!endp) return lstr_get("");
    const size_t offset = endp - filename + 1;
    return Mid(filename, offset, strlen(filename) - offset);
}

const TChar* ExtractDir(const TChar* filename) {
    const TChar* fendp = strrchr(filename, '/');
    const TChar* bendp = strrchr(filename, '\\');
    const TChar* endp = (fendp >= bendp) ? fendp : bendp;
    if (!endp) return lstr_get("");
    const size_t size = endp - filename;
    return Mid(filename, 0, size);
}

TInt Asc(const TChar* str, TInt index) {
    return (TInt)str[index];
}

const TChar* Chr(TInt c) {
    const TChar str[] = {(TChar)c, '\0'};
    return lstr_get(str);
}

const TChar* Str(TInt val) {
    TChar str[64];
#ifdef ENV64
    sprintf(str, "%lli", val);
#else
    sprintf(str, "%i", val);
#endif
    return lstr_get(str);
}

const TChar* StrF(TFloat val) {
    TChar str[64];
#ifdef ENV64
    sprintf(str, "%lf", val);
#else
    sprintf(str, "%f", val);
#endif
    return lstr_get(str);
}

TInt Val(const TChar* str) {
    TInt val = 0;
#ifdef ENV64
    sscanf(str, "%lli", &val);
#else
    sscanf(str, "%i", &val);
#endif
    return val;
}

TFloat ValF(const TChar* str) {
    TFloat val = 0;
#ifdef ENV64
    sscanf(str, "%lf", &val);
#else
    sscanf(str, "%f", &val);
#endif
    return val;
}

const TChar* LoadString(const TChar* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return lstr_get("");
    fseek(f, 0, SEEK_END);
    const long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    TChar* buf = (TChar*)malloc(size+1);
    fread(buf, sizeof(TChar), size, f);
    buf[size] = '\0';
    const TChar* result = lstr_get(buf);
    free(buf);
    return result;
}

void SaveString(const TChar* filename, const TChar* str, TInt append) {
    FILE* f = fopen(filename, append ? "ab" : "wb");
    if (!f) return;
    fwrite(str, sizeof(TChar), strlen(str), f);
    fclose(f);
}

// ------------------------------------
// Callable
// ------------------------------------

void AddIntArg(TInt arg) {
}

void AddFloatArg(TFloat arg) {
}

void AddStringArg(const TChar* arg) {
}

void Call(const TChar* name) {
}

TInt CallInt(const TChar* name) {
    return 0;
}

TFloat CallFloat(const TChar* name) {
    return 0.0f;
}

const TChar* CallString(const TChar* name) {
    return lstr_get("");
}

TInt Callable(const TChar* name) {
    return 0;
}
