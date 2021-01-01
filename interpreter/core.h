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

// ------------------------------------
// App
// ------------------------------------

const char* AppName();
size_t AppArgs(size_t table);
const char* Run(const char* command);

// ------------------------------------
// Console
// ------------------------------------

const char* Input(const char* prompt);
void Print(const char* msg);

// ------------------------------------
// Dir
// ------------------------------------

size_t DirContents(size_t table, const char* path);
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

size_t DimTable(size_t index);
void UndimTable(size_t table);
void SetTableInt(size_t table, const char* key, int value);
void SetTableFloat(size_t table, const char* key, float value);
void SetTableString(size_t table, const char* key, const char* value);
int TableInt(const size_t table, const char* key);
float TableFloat(const size_t table, const char* key);
const char* TableString(const size_t table, const char* key);
int Contains(const size_t table, const char* key);
void Remove(size_t table, const char* key);
int Size(const size_t table);
void Clear(size_t table);

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

#ifndef SWIG
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
#endif
