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

#ifndef CORE_IMPL
typedef void Memory;
#else
struct Memory;
#endif
struct Table;

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------
// App
// ------------------------------------

const char* AppName();
Table* AppArgs();
const char* Run(const char* command);
void* _IncRef(void* ptr);
void _DecRef(void* ptr);
void* _AutoDec(void* ptr);
void _DoAutoDec();

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

Memory* Dim(int size);
void Undim(Memory* mem);
void Redim(Memory* mem, int size);
Memory* LoadDim(const char* filename);
void SaveDim(Memory* mem, const char* filename);
int DimSize(Memory* mem);
int PeekByte(Memory* mem, int offset);
int PeekShort(Memory* mem, int offset);
int PeekInt(Memory* mem, int offset);
float PeekReal(Memory* mem, int offset);
const char* PeekString(Memory* mem, int offset);
void* PeekRef(Memory* mem, int offset);
void PokeByte(Memory* mem, int offset, int val);
void PokeShort(Memory* mem, int offset, int val);
void PokeInt(Memory* mem, int offset, int val);
void PokeReal(Memory* mem, int offset, float val);
void PokeString(Memory* mem, int offset, const char* val);
void PokeRef(Memory* mem, int offset, void* val);

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
const char* Join(Table* table, const char* separator);
Table* Split(const char* str, const char* separator);
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
void SetTableInt(Table* table, const char* key, int value);
void SetTableReal(Table* table, const char* key, float value);
void SetTableString(Table* table, const char* key, const char* value);
void SetTableTable(Table* table, const char* key, Table* value);
void SetTableRef(Table* table, const char* key, void* value);
int TableInt(const Table* table, const char* key);
float TableReal(const Table* table, const char* key);
const char* TableString(const Table* table, const char* key);
Table* TableTable(const Table* table, const char* key);
void* TableRef(const Table* table, const char* key);
void SetIndexInt(Table* table, size_t index, int value);
void SetIndexReal(Table* table, size_t index, float value);
void SetIndexString(Table* table, size_t index, const char* value);
void SetIndexTable(Table* table, size_t index, Table* value);
void SetIndexRef(Table* table, size_t index, void* value);
int IndexInt(const Table* table, size_t index);
float IndexReal(const Table* table, size_t index);
const char* IndexString(const Table* table, size_t index);
Table* IndexTable(const Table* table, size_t index);
void* IndexRef(const Table* table, size_t index);
int Contains(const Table* table, const char* key);
void Remove(Table* table, const char* key);
int Size(const Table* table);
void Clear(Table* table);

// ------------------------------------
// Callable
// ------------------------------------

void AddIntArg(int arg);
void AddRealArg(float arg);
void AddStringArg(const char* arg);
void Call(const char* name);
int CallInt(const char* name);
float CallReal(const char* name);
const char* CallString(const char* name);
int Callable(const char* name);

#ifdef __cplusplus
}
#endif
