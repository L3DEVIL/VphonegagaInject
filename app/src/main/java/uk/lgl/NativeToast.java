package uk.lgl;

import android.content.Context;

public class NativeToast {
    public static native void Toast(Context context, int numOfMessage);

    public static void makeText(final Context context, int numOfMessage) {
        Toast(context, numOfMessage);
    }
}
