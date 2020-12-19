#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#ifdef FINDERLIB_EXPORTS
#define FINDERLIB_API __declspec(dllexport)
#else
#define FINDERLIB_API __declspec(dllimport)
#endif


//extern "C" FINDERLIB_API void fibonacci_init(
//    const unsigned long long a, const unsigned long long b);
//
//
//extern "C" FINDERLIB_API bool fibonacci_next();
//
//
//extern "C" FINDERLIB_API unsigned long long fibonacci_current();
//
//
//extern "C" FINDERLIB_API unsigned fibonacci_index();
extern "C" FINDERLIB_API void Index(const wchar_t*);
extern "C" FINDERLIB_API std::string Next(std::wstring fileName, std::string findWord);
extern "C" FINDERLIB_API std::string Prev(std::wstring fileName, std::string findWord);
extern "C" FINDERLIB_API  std::string ReadFileMem(std::wstring, std::string);

extern "C" FINDERLIB_API std::string Find(std::string, std::string);
