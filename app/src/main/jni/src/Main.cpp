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
    MemoryPatch GodMode, SpeedHack;
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
JNIEXPORT jobjectArray
JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;
    featureListValid = true;

    // Note: Do not translate the first text
    // Usage:
    // Category_(text)
    // Toggle_[feature name]
    // SeekBar_[feature name]_[min value]_[max value]
    // Spinner_[feature name]_[Items e.g. item1_item2_item3]
    // Button_[feature name]
    // Button_OnOff_[feature name]
    // InputValue_[feature name]
    const char *features[] = {
            OBFUSCATE("Category_The Category"),
            OBFUSCATE("Toggle_The toggle"),
            OBFUSCATE("SeekBar_The slider_0_100"),
            OBFUSCATE("Spinner_The spinner_Items 1_Items 2_Items 3"),
            OBFUSCATE("Button_The button"),
            OBFUSCATE("Button_OnOff_The On/Off button"),
            OBFUSCATE("InputValue_The input number")
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]); //Now you dont have to manually update the number everytime;
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint feature, jint value, jboolean boolean, jstring str) {

    const char *featureName = env->GetStringUTFChars(str, 0);
    feature += 1;  // No need to count from 0 anymore. yaaay :)))

    LOGD(OBFUSCATE("Feature name: [%d] %s | Value: = %d | Bool: = %d"), feature, featureName, value, boolean);

    // Changed to if-statement because modders can easly mess up with cases.
    if (feature == 1) {
        // The category was 1 so is not used
    }
    else if (feature == 2) {
        feature2 = boolean;
        if (feature2) {
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
    }
    else if (feature == 3) {
        sliderValue = value;
    }
    else if (feature == 4) {
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
    }
    else if (strcmp(featureName, "The button") == 0) { //Works with string too
        LOGD(OBFUSCATE("Feature 5 Called"));
        //Since we have instanceBtn as a field, we can call it out of Update hook function
        if (instanceBtn != NULL)
            AddMoneyExample(instanceBtn, 999999);
        MakeToast(env, obj, OBFUSCATE("Button pressed"), Toast::LENGTH_SHORT);
    }
    else if (strcmp(featureName, "The On/Off button") == 0) {
        LOGD(OBFUSCATE("Feature 6 Called"));
        featureHookToggle = boolean;
    }
    else if (feature == 7) {

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
}*/
