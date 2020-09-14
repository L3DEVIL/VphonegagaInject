#ifndef LGL_IL2CPP_MENU_TOAST_H
#define LGL_IL2CPP_MENU_TOAST_H

void ShowToast(JNIEnv *env, jobject obj) {
    MakeToast(env, obj, OBFUSCATE("Modded by LGL"), Toast::LENGTH_LONG);
    isToastCalled = true;
}

extern "C" {
JNIEXPORT void JNICALL
Java_uk_lgl_loadlib_LoadLib_Toast(JNIEnv *env, jobject thiz, jobject
obj) {
    ShowToast(env, obj);
}

JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_ToastStartup(JNIEnv *env, jobject thiz) {
    ShowToast(env, thiz);
}
}

#endif //LGL_IL2CPP_MENU_TOAST_H
