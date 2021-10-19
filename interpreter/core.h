#pragma once

#include "../src/common.h"

#ifdef _WIN32
#ifdef DeleteFile
#undef DeleteFile
#endif
#ifdef LoadString
#undef LoadString
#endif
#endif

struct Table;

// ------------------------------------
// App
// ------------------------------------

const char* AppName();
Table* AppArgs();
const char* Run(const char* command);

// ------------------------------------
// Console
// ------------------------------------

const char* Input(const char* prompt);
void Print(const char* msg);

// ------------------------------------
// Dir
// ------------------------------------

Table* DirContents(const char* path);
const char* CurrentDir();
void ChangeDir(const char* dir);
const char* FullPath(const char* filename);

// ------------------------------------
// File
// ------------------------------------

int FileType(const char* filename);
void DeleteFile(const char* filename);

// ------------------------------------
// Math
// ------------------------------------

float ASin(float x);
float ATan(float x);
float ATan2(float x, float y);
float Abs(float x);
float Ceil(float x);
float Clamp(float x, float min, float max);
float Cos(float x);
float Exp(float x);
float Floor(float x);
float Log(float x);
float Max(float x, float y);
float Min(float x, float y);
float Pow(float x, float y);
float Sgn(float x);
float Sin(float x);
float Sqrt(float x);
float Tan(float x);
int Int(float num);

// ------------------------------------
// Memory
// ------------------------------------

size_t Dim(size_t index, int size);
void Undim(size_t index);
void Redim(size_t index, int size);
size_t LoadDim(size_t index, const char* filename);
void SaveDim(size_t index, const char* filename);
int DimSize(size_t index);
int PeekByte(size_t index, int offset);
int PeekShort(size_t index, int offset);
int PeekInt(size_t index, int offset);
float PeekFloat(size_t index, int offset);
const char* PeekString(size_t index, int offset);
void PokeByte(size_t index, int offset, int val);
void PokeShort(size_t index, int offset, int val);
void PokeInt(size_t index, int offset, int val);
void PokeFloat(size_t index, int offset, float val);
void PokeString(size_t index, int offset, const char* val);

// ------------------------------------
// String
// ------------------------------------

int Len(const char* str);
const char* Left(const char* str, int count);
const char* Right(const char* str, int count);
const char* Mid(const char* str, int offset, int count);
const char* Lower(const char* str);
const char* Upper(const char* str);
int Find(const char* str, const char* find, int offset);
const char* Replace(const char* str, const char* find, const char* replace);
const char* Trim(const char* str);
const char* Join(size_t table, const char* separator);
size_t Split(size_t table, const char* str, const char* separator);
const char* StripExt(const char* filename);
const char* StripDir(const char* filename);
const char* ExtractExt(const char* filename);
const char* ExtractDir(const char* filename);
int Asc(const char* str, int index);
const char* Chr(int c);
const char* Str(int val);
const char* StrF(float val);
int Val(const char* str);
float ValF(const char* str);
const char* LoadString(const char* filename);
void SaveString(const char* filename, const char* str, int append);

// ------------------------------------
// Table
// ------------------------------------

Table* DimTable();
void UndimTable(Table* table);
void SetTableInt(Table* table, const char* key, int value);
void SetTableFloat(Table* table, const char* key, float value);
void SetTableString(Table* table, const char* key, const char* value);
void SetTableRef(Table* table, const char* key, void* value);
int TableInt(const Table* table, const char* key);
float TableFloat(const Table* table, const char* key);
const char* TableString(const Table* table, const char* key);
void* TableRef(const Table* table, const char* key);
void SetIndexInt(Table* table, size_t index, int value);
void SetIndexFloat(Table* table, size_t index, float value);
void SetIndexString(Table* table, size_t index, const char* value);
void SetIndexRef(Table* table, size_t index, void* value);
int IndexInt(const Table* table, size_t index);
float IndexFloat(const Table* table, size_t index);
const char* IndexString(const Table* table, size_t index);
void* IndexRef(const Table* table, size_t index);
int Contains(const Table* table, const char* key);
void Remove(Table* table, const char* key);
int Size(const Table* table);
void Clear(Table* table);

// ------------------------------------
// Callable
// ------------------------------------

void AddIntArg(int arg);
void AddFloatArg(float arg);
void AddStringArg(const char* arg);
void Call(const char* name);
int CallInt(const char* name);
float CallFloat(const char* name);
const char* CallString(const char* name);
int Callable(const char* name);

// ------------------------------------
// Resource pools
// ------------------------------------

template<typename T>
class Pool {
public:
    Pool(void (* deleter)(T) = NULL) : deleter(deleter) {
    }

    size_t Insert(size_t index, const T& object) {
        if (Exists(index)) {
            Remove(index);
        }
        index = FixIndex(index);
        objects[index] = object;
        return index;
    }

    void Remove(size_t index) {
        if (Exists(index)) {
            if (deleter) deleter(objects[index]);
            objects.erase(index);
        }
    }

    bool Exists(size_t index) const {
        return index >= 0 && objects.count(index) > 0;
    }

    T& Get(size_t index) {
        static T dummy;
        if (Exists(index)) {
            return objects[index];
        } else {
            return dummy;
        }
    }
private:
    std::map<size_t, T> objects;
    void (* deleter)(T);

    size_t FixIndex(size_t index) const {
        if (index == -1) {
            while (true) {
                if (!Exists(++index)) {
                    return index;
                }
            }
        } else {
            return index;
        }
    }
};
