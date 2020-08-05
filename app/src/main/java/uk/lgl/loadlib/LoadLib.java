package uk.lgl.loadlib;

import android.content.Context;
import android.os.Handler;

public class LoadLib {
    private native void Toast(Context context);

    public static void Start(final Context context) {
        // When you change the lib name, change also on Android.mk file
        // Both must have same name
        System.loadLibrary("MyLibName");

        new Handler().postDelayed(new Runnable() {
            /* class com.loadLib.libLoader.AnonymousClass1 */

            public void run() {
                //We need getApplicationContext() to fix background issue with toast
                LoadLib instance = new LoadLib();
                instance.Toast(context.getApplicationContext());
            }
        }, 2000);
    }
}
