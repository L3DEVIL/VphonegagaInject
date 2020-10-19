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
#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include "src/Includes/base64.hpp"
#include "src/KittyMemory/MemoryPatch.h"
#include "src/Includes/Logger.h"
#include "src/Includes/Utils.h"
#include "src/Includes/obfuscate.h"

#include "Menu/Sounds.h"
#include "Menu/Menu.h"

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
    MemoryPatch GodMode, SliderExample;
    // etc...
} hexPatches;

bool feature1 = false, feature2 = false, featureHookToggle = false;
int sliderValue = 0;
void *instanceBtn;

// Function pointer splitted because we want to avoid crash when the il2cpp lib isn't loaded.
// If you putted getAbsoluteAddress here, the lib tries to read the address without il2cpp loaded,
// will result in a null pointer which will cause crash
// See https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/
void (*AddMoneyExample)(void *instance, int amount);

extern "C" {
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint feature, jint value, jboolean boolean, jstring str) {

    const char *featureName = env->GetStringUTFChars(str, 0);
    feature += 1;  // No need to count from 0 anymore. yaaay :)))

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d"), feature, featureName, value,
                       boolean);

    // Changed to if-statement because modders can easly mess up with cases.
    if (feature == 1) {
        // The category was 1 so is not used
    } else if (feature == 2) {
        feature2 = boolean;
        if (feature2) {
            // MakeToast(env, obj, OBFUSCATE("Feature 1 ON"), Toast::LENGTH_SHORT);
            // modify & print bytes
            if (hexPatches.GodMode.Modify()) {
                LOGD(OBFUSCATE("Current Bytes: %s"),
                     hexPatches.GodMode.get_CurrBytes().c_str());
            }
            //Or
            hexPatches.GodMode.Modify();
        } else {
            //MakeToast(env, obj, OBFUSCATE("Feature 1 OFF"), Toast::LENGTH_SHORT);
            //restore & print bytes
            if (hexPatches.GodMode.Restore()) {
                LOGD(OBFUSCATE("Current Bytes: %s"),
                     hexPatches.GodMode.get_CurrBytes().c_str());
            }
            //Or
            hexPatches.GodMode.Restore();
        }
    } else if (feature == 3) {
        sliderValue = value;
    } else if (feature == 4) {
        //Yes you can do kittymemory patches in a slider
        if (value == 0) {
            hexPatches.SliderExample = MemoryPatch::createWithHex(
                    libName, string2Offset(
                            OBFUSCATE_KEY("0x100000", '-')),
                    OBFUSCATE(
                            "00 00 A0 E3 1E FF 2F E1"));
            hexPatches.SliderExample.Modify();
        }
        if (value == 1) {
            hexPatches.SliderExample = MemoryPatch::createWithHex(
                    libName, string2Offset(
                            OBFUSCATE_KEY("0x100000",
                                          '-')),
                    OBFUSCATE(
                            "01 00 A0 E3 1E FF 2F E1"));
            hexPatches.SliderExample.Modify();
        }
        if (value == 2) {
            hexPatches.SliderExample = MemoryPatch::createWithHex(
                    libName,
                    string2Offset(
                            OBFUSCATE_KEY("0x100000",
                                          '-')),
                    OBFUSCATE(
                            "02 00 A0 E3 1E FF 2F E1"));
            hexPatches.SliderExample.Modify();
        }
        if (value == 3) {
            //...
        }
        if (value == 4) {
            //...
        }
    } else if (feature == 5) {
        if (value == 0) {
            LOGD(OBFUSCATE("Selected item 1"));
        }
        if (value == 1) {
            LOGD(OBFUSCATE("Selected item 1"));
        }
        if (value == 2) {
            LOGD(OBFUSCATE("Selected item 3"));
        }
    } else if (strcmp(featureName, "The button") == 0) { //Works with string too
        LOGD(OBFUSCATE("Feature 6 Called"));
        //Since we have instanceBtn as a field, we can call it out of Update hook function
        if (instanceBtn != NULL)
            AddMoneyExample(instanceBtn, 999999);
        MakeToast(env, obj, OBFUSCATE("Button pressed"), Toast::LENGTH_SHORT);
    } else if (strcmp(featureName, "The On/Off button") == 0) {
        LOGD(OBFUSCATE("Feature 7 Called"));
        featureHookToggle = boolean;
    } else if (feature == 7) {

    }

    //You can still do cases if you prefer that but careful not to remove break; by accidently
    /*switch (feature) {
        case 0:
            feature1 = boolean;
            break;
        case 1:
            feature2 = boolean;
            break;
        case 2:
            //etc.
            break;
        case 3:
            //etc.
            break;
    }*/
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

void (*old_Update)(void *instance);

void Update(void *instance) {
    instanceBtn = instance;
    old_Update(instance);
}

// we will run our patches in a new thread so our while loop doesn't block process main thread
// Don't forget to remove or comment out logs before you compile it.

//KittyMemory Android Example: https://github.com/MJx0/KittyMemory/blob/master/Android/test/src/main.cpp
//Note: We use OBFUSCATE_KEY for offsets which is the important part xD
void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread called"));

    //Default lib target is Il2Cpp. Uncomment if you want to target other lib
    //libName = OBFUSCATE("libOtherLib.so");

    //Check if target lib is loaded
    do {
        sleep(1);
    } while (!isLibraryLoaded(libName));
    LOGI(OBFUSCATE("Lib %s has been loaded"), libName);

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

    // Function pointer splitted because we want to avoid crash when the il2cpp lib isn't loaded.
    // See https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/
    AddMoneyExample = (void(*)(void *,int))getAbsoluteAddress(libName, 0x123456);

#else //Compile for armv7 lib only. Do not worry about greyed out highlighting code, it still works

    // New way to patch hex via KittyMemory without need to specify len. Spaces or without spaces are fine
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

    // Function pointer splitted because we want to avoid crash when the il2cpp lib isn't loaded.
    // See https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/
    AddMoneyExample = (void (*)(void *, int)) getAbsoluteAddress(libName, 0x123456);

    LOGI(OBFUSCATE("Hooked"));
#endif

    return NULL;
}

//No need to use JNI_OnLoad, since we don't use JNIEnv
//We do this to hide OnLoad from disassembler
__attribute__((constructor))
void lib_main() {
    LOGI(OBFUSCATE("Own lib has been loaded"));
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);

    //Run anti-leech
    pthread_t p;
    pthread_create(&p, NULL, antiLeech, NULL);
}

/*
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);

    return JNI_VERSION_1_6;
}

//Does not work yet
//\ndk\21.3.6528147\sources\android\native_app_glue
//#include <android_native_app_glue.h>

void android_main(struct android_app* state) {
    LOGI(OBFUSCATE("android_main"));
}
 */
