/*
 * Credits:
 *
 * Octowolve - Mod menu: https://github.com/z3r0Sec/Substrate-Template-With-Mod-Menu
 * And hooking: https://github.com/z3r0Sec/Substrate-Hooking-Example
 * VanHoevenTR A.K.A Nixi: https://github.com/LGLTeam/VanHoevenTR_Android_Mod_Menu
 * MrIkso - Mod menu: https://github.com/MrIkso/FloatingModMenu
 * Rprop - https://github.com/Rprop/And64InlineHook
 * MJx0 A.K.A Ruit - KittyMemory: https://github.com/MJx0/KittyMemory
 * */

#include <pthread.h>
#include <jni.h>
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Includes/obfuscate.h"
#include "Toast.h"

#if defined(__aarch64__) //Compile for arm64 lib only
#include <src/And64InlineHook/And64InlineHook.hpp>
#else //Compile for armv7 lib only. Do not worry about greyed out highlighting code, it still works

#include <src/Substrate/SubstrateHook.h>
#include <src/Substrate/CydiaSubstrate.h>

#endif

// fancy struct for patches for kittyMemory
struct My_Patches {
    // let's assume we have patches for these functions for whatever game
    // like show in miniMap boolean function
    MemoryPatch GodMode, SpeedHack;
    // etc...
} hexPatches;

bool feature1 = false, featureHookToggle = false;
int sliderValue = 0;

extern "C" {
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_Changes(JNIEnv *env, jobject obj,
                                                   jint feature, jint value) {
    LOGD(OBFUSCATE("Feature: = %d | Int value: = %d"), feature, value);

    // You must count your features from 0 (YES, I MEAN ZERO), not 1
    switch (feature) {
        // The category was 0 so "case 0" is not used
        case 0:
            break;
        case 1: // Switch
            feature1 = !feature1;
            if (feature1) {
                MakeToast(env, obj, OBFUSCATE("Feature 1 ON"), Toast::LENGTH_SHORT);
                // modify & print bytes
                if (hexPatches.GodMode.Modify()) {
                    LOGD(OBFUSCATE("Current Bytes: %s"),
                         hexPatches.GodMode.get_CurrBytes().c_str());
                }
                //Or
                hexPatches.GodMode.Modify();
            } else {
                MakeToast(env, obj, OBFUSCATE("Feature 1 OFF"), Toast::LENGTH_SHORT);
                //restore & print bytes
                if (hexPatches.GodMode.Restore()) {
                    LOGD(OBFUSCATE("Current Bytes: %s"),
                         hexPatches.GodMode.get_CurrBytes().c_str());
                }
                //Or
                hexPatches.GodMode.Restore();
            }
            break;
        case 2: // Slider in hook example
            sliderValue = value;
            break;
        case 3: // Spinner
            switch (value) {
                case 0:
                    MakeToast(env, obj, OBFUSCATE("Selected item 1"), Toast::LENGTH_SHORT);
                    break;
                case 1:
                    MakeToast(env, obj, OBFUSCATE("Selected item 2"), Toast::LENGTH_SHORT);
                    break;
                case 2:
                    MakeToast(env, obj, OBFUSCATE("Selected item 3"), Toast::LENGTH_SHORT);
                    break;
            }
            break;
        case 4: // Button
            MakeToast(env, obj, OBFUSCATE("Button pressed"), Toast::LENGTH_SHORT);
            break;
        case 5:
            featureHookToggle = !featureHookToggle;
            break;
    }
}
}

// ---------- Hooking ---------- //

bool (*old_get_BoolExample)(void *instance);

bool get_BoolExample(void *instance) {
    if (instance != NULL && featureHookToggle) {
        return true;
    }
    return old_get_BoolExample(instance);
}

float (*old_get_FloatExample)(void *instance);

float get_FloatExample(void *instance) {
    if (instance != NULL && sliderValue > 1) {
        return (float) sliderValue;
    }
    return old_get_FloatExample(instance);
}

#define libName OBFUSCATE("libil2cpp.so")

// we will run our patches in a new thread so our while loop doesn't block process main thread
// Don't forget to remove or comment out logs before you compile it.

//KittyMemory Android Example: https://github.com/MJx0/KittyMemory/blob/master/Android/test/src/main.cpp
//Note: We use OBFUSCATE_KEY for offsets which is the important part xD
void *hack_thread(void *) {
    LOGI(OBFUSCATE("Lib have been loaded"));


#if defined(__aarch64__) //Compile for arm64 lib only
    // New way to patch hex via KittyMemory without need to  specify len. Spaces or without spaces are fine
    hexPatches.GodMode = MemoryPatch::createWithHex(libName,
                                                    string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                    OBFUSCATE("00 00 A0 E3 1E FF 2F E1"));

    // Offset Hook example
    A64HookFunction((void *) getAbsoluteAddress(libName, string2Offset(OBFUSCATE_KEY("0x123456", 'l'))), (void *) get_BoolExample,
                    (void **) &old_get_BoolExample);

    // Symbol hook example (untested). Symbol/function names can be found in IDA if the lib are not stripped. This is not for il2cpp games
    A64HookFunction((void *)("__SymbolNameExample"), (void *) get_BoolExample,
                   (void **) &old_get_BoolExample);

#else //Compile for armv7 lib only. Do not worry about greyed out highlighting code, it still works

    // New way to patch hex via KittyMemory without need to  specify len. Spaces or without spaces are fine
    hexPatches.GodMode = MemoryPatch::createWithHex(libName,
                                                    string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                    OBFUSCATE("00 00 A0 E3 1E FF 2F E1"));

    // Offset Hook example
    MSHookFunction(
            (void *) getAbsoluteAddress(libName, string2Offset(OBFUSCATE_KEY("0x123456", '?'))),
            (void *) get_BoolExample,
            (void **) &old_get_BoolExample);

    // Symbol hook example (untested). Symbol/function names can be found in IDA if the lib are not stripped. This is not for il2cpp games
    MSHookFunction((void *) ("__SymbolNameExample"), (void *) get_BoolExample,
                   (void **) &old_get_BoolExample);

    LOGI(OBFUSCATE("Hooked"));
#endif

    //Call bad function if isToastCalled is false
    sleep(15);
    if (!isToastCalled){
        int *p = 0;
        *p = 0;
    }

    return NULL;
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);

    return JNI_VERSION_1_6;
}