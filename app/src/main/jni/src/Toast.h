#ifndef LGL_IL2CPP_MENU_TOAST_H
#define LGL_IL2CPP_MENU_TOAST_H

extern "C" {
JNIEXPORT void JNICALL
Java_uk_lgl_NativeToast_Toast(JNIEnv *env, jclass obj, jobject context, jint numOfMessage) {
    if (numOfMessage == 0){
        MakeToast(env, context, OBFUSCATE("Modded by LGL"), Toast::LENGTH_LONG);
    }
    if (numOfMessage == 1){
        MakeToast(env, context, OBFUSCATE("Icon hidden. Remember the hidden icon position"), Toast::LENGTH_LONG);
    }
    if (numOfMessage == 2){
        MakeToast(env, context, OBFUSCATE("Menu service killed"), Toast::LENGTH_LONG);
    }
    isToastCalled = true;
}
}

#endif //LGL_IL2CPP_MENU_TOAST_H
