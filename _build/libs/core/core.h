#ifndef CORE_H
#define CORE_H

#if _WIN32 || _WIN64
#if _WIN64
#define ENV64
#else
#define ENV32
#endif
#else
#if __x86_64__ || __ppc64__ || __aarch64__
#define ENV64
#else
#define ENV32
#endif
#endif

#define TYPE_INT -1
#define TYPE_FLOAT -2
#define TYPE_STRING -3
#define TYPE_LIST -4
#define TYPE_DICT -5
#define TYPE_RAW -6
#define TYPE_VOID -7

#ifdef _WIN32
#ifdef DeleteFile
#undef DeleteFile
#endif
#ifdef LoadString
#undef LoadString
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENV64
typedef long long int TInt;
typedef double TFloat;
typedef char TChar;
#else
typedef long int TInt;
typedef float TFloat;
typedef char TChar;
#endif

#ifndef CORE_IMPL
typedef void TMemory;
#else
struct TMemory;
#endif
struct TList;
struct TDict;

// ------------------------------------
// App
// ------------------------------------

const TChar* AppName();
struct TList* AppArgs();
const TChar* Run(const TChar* command);
void* _IncRef(void* ptr);
void _DecRef(void* ptr);
void* _AutoDec(void* ptr);
void _DoAutoDec();

// ------------------------------------
// Console
// ------------------------------------

const TChar* Input(const TChar* prompt);
void Print(const TChar* msg);

// ------------------------------------
// Dir
// ------------------------------------

struct TList* DirContents(const TChar* path);
const TChar* CurrentDir();
void ChangeDir(const TChar* dir);
const TChar* FullPath(const TChar* filename);

// ------------------------------------
// File
// ------------------------------------

TInt FileType(const TChar* filename);
void DeleteFile(const TChar* filename);

// ------------------------------------
// TList
// ------------------------------------

struct TList* _CreateList();
struct TList* _SetListInt(struct TList* list, size_t index, TInt value);
struct TList* _SetListFloat(struct TList* list, size_t index, TFloat value);
struct TList* _SetListString(struct TList* list, size_t index, const TChar* value);
struct TList* _SetListList(struct TList* list, size_t index, struct TList* value);
struct TList* _SetListDict(struct TList* list, size_t index, struct TDict* value);
struct TList* _SetListRef(struct TList* list, size_t index, void* value);
TInt _ListInt(struct TList* list, size_t index);
TFloat _ListFloat(struct TList* list, size_t index);
const TChar* _ListString(struct TList* list, size_t index);
struct TList* _ListList(struct TList* list, size_t index);
struct TDict* _ListDict(struct TList* list, size_t index);
void* _ListRef(struct TList* list, size_t index);
const TChar* _ListToString(struct TList* list);
void RemoveIndex(struct TList* list, TInt index);
TInt ListSize(struct TList* list);
void ClearList(struct TList* list);

// ------------------------------------
// TDict
// ------------------------------------

struct TDict* _CreateDict();
struct TDict* _SetDictInt(struct TDict* dict, const TChar* key, TInt value);
struct TDict* _SetDictFloat(struct TDict* dict, const TChar* key, TFloat value);
struct TDict* _SetDictString(struct TDict* dict, const TChar* key, const TChar* value);
struct TDict* _SetDictList(struct TDict* dict, const TChar* key, struct TList* value);
struct TDict* _SetDictDict(struct TDict* dict, const TChar* key, struct TDict* value);
struct TDict* _SetDictRef(struct TDict* dict, const TChar* key, void* value);
TInt _DictInt(struct TDict* dict, const TChar* key);
TFloat _DictFloat(struct TDict* dict, const TChar* key);
const TChar* _DictString(struct TDict* dict, const TChar* key);
struct TList* _DictList(struct TDict* dict, const TChar* key);
struct TDict* _DictDict(struct TDict* dict, const TChar* key);
void* _DictRef(struct TDict* dict, const TChar* key);
const TChar* _DictToString(struct TDict* dict);
TInt Contains(struct TDict* dict, const TChar* key);
void RemoveKey(struct TDict* dict, const TChar* key);
TInt DictSize(struct TDict* dict);
void ClearDict(struct TDict* dict);

// ------------------------------------
// Math
// ------------------------------------

TFloat ASin(TFloat x);
TFloat ATan(TFloat x);
TFloat ATan2(TFloat x, TFloat y);
TFloat Abs(TFloat x);
TFloat Ceil(TFloat x);
TFloat Clamp(TFloat x, TFloat min, TFloat max);
TFloat Cos(TFloat x);
TFloat Exp(TFloat x);
TFloat Floor(TFloat x);
TFloat Log(TFloat x);
TFloat Max(TFloat x, TFloat y);
TFloat Min(TFloat x, TFloat y);
TFloat Pow(TFloat x, TFloat y);
TFloat Sgn(TFloat x);
TFloat Sin(TFloat x);
TFloat Sqrt(TFloat x);
TFloat Tan(TFloat x);
TInt Int(TFloat num);

// ------------------------------------
// TMemory
// ------------------------------------

TMemory* Dim(TInt size);
void Undim(TMemory* mem);
void Redim(TMemory* mem, TInt size);
TMemory* LoadDim(const TChar* filename);
void SaveDim(TMemory* mem, const TChar* filename);
TInt DimSize(TMemory* mem);
TInt PeekByte(TMemory* mem, TInt offset);
TInt PeekShort(TMemory* mem, TInt offset);
TInt PeekInt(TMemory* mem, TInt offset);
TFloat PeekFloat(TMemory* mem, TInt offset);
const TChar* PeekString(TMemory* mem, TInt offset);
void* PeekRef(TMemory* mem, TInt offset);
void PokeByte(TMemory* mem, TInt offset, TInt val);
void PokeShort(TMemory* mem, TInt offset, TInt val);
void PokeInt(TMemory* mem, TInt offset, TInt val);
void PokeFloat(TMemory* mem, TInt offset, TFloat val);
void PokeString(TMemory* mem, TInt offset, const TChar* val);
void PokeRef(TMemory* mem, TInt offset, void* val);

// ------------------------------------
// String
// ------------------------------------

TInt Len(const TChar* str);
const TChar* Left(const TChar* str, TInt count);
const TChar* Right(const TChar* str, TInt count);
const TChar* Mid(const TChar* str, TInt offset, TInt count);
const TChar* Lower(const TChar* str);
const TChar* Upper(const TChar* str);
TInt Find(const TChar* str, const TChar* find, TInt offset);
const TChar* Replace(const TChar* str, const TChar* find, const TChar* replace);
const TChar* Trim(const TChar* str);
const TChar* Join(struct TList* list, const TChar* separator);
struct TList* Split(const TChar* str, const TChar* separator);
const TChar* StripExt(const TChar* filename);
const TChar* StripDir(const TChar* filename);
const TChar* ExtractExt(const TChar* filename);
const TChar* ExtractDir(const TChar* filename);
TInt Asc(const TChar* str, TInt index);
const TChar* Chr(TInt c);
const TChar* Str(TInt val);
const TChar* StrF(TFloat val);
TInt Val(const TChar* str);
TFloat ValF(const TChar* str);
const TChar* LoadString(const TChar* filename);
void SaveString(const TChar* filename, const TChar* str, TInt append);

// ------------------------------------
// Callable
// ------------------------------------

void AddIntArg(TInt arg);
void AddFloatArg(TFloat arg);
void AddStringArg(const TChar* arg);
void Call(const TChar* name);
TInt CallInt(const TChar* name);
TFloat CallFloat(const TChar* name);
const TChar* CallString(const TChar* name);
TInt Callable(const TChar* name);

#ifdef __cplusplus
}
#endif

#endif // CORE_H
