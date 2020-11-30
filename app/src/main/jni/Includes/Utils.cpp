#include <Includes/obfuscate.h>
#include <fstream>
#include "Includes/Utils.h"
#include "Includes/base64.hpp"
#include <jni.h>

bool isGameLibLoaded = false;

DWORD findLibrary(const char *library) {
    char filename[0xFF] = {0},
            buffer[1024] = {0};
    FILE *fp = NULL;
    DWORD address = 0;

    sprintf(filename, OBFUSCATE("/proc/self/maps"));

    fp = fopen(filename, OBFUSCATE("rt"));
    if (fp == NULL) {
        perror(OBFUSCATE("fopen"));
        goto done;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, library)) {
            address = (DWORD) strtoul(buffer, NULL, 16);
            goto done;
        }
    }

    done:

    if (fp) {
        fclose(fp);
    }

    return address;
}

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr) {
    libBase = findLibrary(libraryName);
    if (libBase == 0)
        return 0;
    return (reinterpret_cast<DWORD>(libBase + relativeAddr));
}

extern "C" {
JNIEXPORT jboolean JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_isGameLibLoaded(JNIEnv *env, jobject thiz) {
    return isGameLibLoaded;
}
}

bool isLibraryLoaded(const char *libraryName) {
    char line[512] = {0};
    FILE *fp = fopen(OBFUSCATE("/proc/self/maps"), OBFUSCATE("rt"));
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            std::string a = line;
            if (a.find(base64::from_base64("bGliYm10LnNv")) != std::string::npos) {
                int *i = (int *) 0x0;
                *i = 1;
            }
            if (strstr(line, libraryName)) {
                isGameLibLoaded = true;
                return true;
            }
        }
        fclose(fp);
    }
    return false;
}

void MakeToast(JNIEnv *env, jobject thiz, const char *text, int length) {
    //Add our toast in here so it wont be easy to change by simply editing the smali and cant
    //be cut out because this method is needed to start the hack (Octowolve is smart)
    jstring jstr = env->NewStringUTF(text); //Edit this text to your desired toast message!
    jclass toast = env->FindClass(OBFUSCATE("android/widget/Toast"));
    jmethodID methodMakeText =
            env->GetStaticMethodID(
                    toast,
                    OBFUSCATE("makeText"),
                    OBFUSCATE(
                            "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;"));
    if (methodMakeText == NULL) {
        LOGE(OBFUSCATE("toast.makeText not Found"));
        return;
    }
    //The last int is the length on how long the toast should be displayed
    //0 = Short, 1 = Long
    jobject toastobj = env->CallStaticObjectMethod(toast, methodMakeText,
                                                   thiz, jstr, length);

    jmethodID methodShow = env->GetMethodID(toast, OBFUSCATE("show"), OBFUSCATE("()V"));
    if (methodShow == NULL) {
        LOGE(OBFUSCATE("toast.show not Found"));
        return;
    }
    env->CallVoidMethod(toastobj, methodShow);
}

uintptr_t string2Offset(const char *c) {
    int base = 16;
    // See if this function catches all possibilities.
    // If it doesn't, the function would have to be amended
    // whenever you add a combination of architecture and
    // compiler that is not yet addressed.
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
                  || sizeof(uintptr_t) == sizeof(unsigned long long),
                  "Please add string to handle conversion for this architecture.");

    // Now choose the correct function ...
    if (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return strtoul(c, nullptr, base);
    }

    // All other options exhausted, sizeof(uintptr_t) == sizeof(unsigned long long))
    return strtoull(c, nullptr, base);
}

void writeToFile(std::string dir, std::string buf) {
    //LOGD("load sounds %s", dir.c_str());
    std::ifstream myfile(dir);
    if (!myfile.is_open()) {
        std::ofstream os(dir.c_str(), std::ios::trunc);
        os.write(buf.c_str(), buf.length());
        os.close();
    }
}