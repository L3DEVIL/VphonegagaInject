package uk.lgl.loadlib;

import android.content.Context;
import android.os.Handler;

import java.lang.annotation.Native;

import uk.lgl.NativeToast;

public class LoadLib {
    public static void Start(final Context context) {
        // When you change the lib name, change also on Android.mk file
        // Both must have same name
        System.loadLibrary("MyLibName");

        new Handler().postDelayed(new Runnable() {
            public void run() {
                NativeToast.makeText(context.getApplicationContext(), 0);
            }
        }, 1000);
    }
}
