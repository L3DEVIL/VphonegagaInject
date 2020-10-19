// Does not work yet

#ifndef ANDROID_MOD_MENU_GETCACHEDIR_H
#define ANDROID_MOD_MENU_GETCACHEDIR_H
std::string android_temp_folder( struct android_app *app ) {
    JNIEnv* env;
    app->activity->vm->AttachCurrentThread( &env, NULL );

    jclass activityClass = env->FindClass( "android/app/NativeActivity" );
    jmethodID getCacheDir = env->GetMethodID( activityClass, "getCacheDir", "()Ljava/io/File;" );
    jobject cache_dir = env->CallObjectMethod( app->activity->clazz, getCacheDir );

    jclass fileClass = env->FindClass( "java/io/File" );
    jmethodID getPath = env->GetMethodID( fileClass, "getPath", "()Ljava/lang/String;" );
    jstring path_string = (jstring)env->CallObjectMethod( cache_dir, getPath );

    const char *path_chars = env->GetStringUTFChars( path_string, NULL );
    std::string temp_folder( path_chars );

    env->ReleaseStringUTFChars( path_string, path_chars );
    app->activity->vm->DetachCurrentThread();
    LOGD("temp_folder.c_str()");
    return temp_folder;
}
#endif //ANDROID_MOD_MENU_GETCACHEDIR_H
