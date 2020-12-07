#ifndef UTILS
#define UTILS

#include <jni.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include "Logger.h"

typedef unsigned long DWORD;
static uintptr_t libBase;

DWORD findLibrary(const char *library);

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr);

bool isLibraryLoaded(const char *libraryName);

void MakeToast(JNIEnv *env, jobject thiz, const char *text, int length);

std::string s(const std::string &str);

uintptr_t string2Offset(const char *c);

namespace Toast {
    inline const int LENGTH_LONG = 1;
    inline const int LENGTH_SHORT = 0;
}

#endif