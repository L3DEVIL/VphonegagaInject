package uk.lgl;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.os.Process;
import android.provider.Settings;
import android.widget.Toast;

import uk.lgl.modmenu.FloatingModMenuService;

public class StaticActivity {

    //Load lib
    static {
        // When you change the lib name, change also on Android.mk file
        // Both must have same name
        System.loadLibrary("MyLibName");
    }

    //Load mod menu
    public static void Start(final Context context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !Settings.canDrawOverlays(context)) {
            Toast.makeText(context, "Overlay permission is required in order to show mod menu", Toast.LENGTH_LONG).show();
            Toast.makeText(context, "Restart the game after you allow permission", Toast.LENGTH_LONG).show();
            context.startActivity(new Intent("android.settings.action.MANAGE_OVERLAY_PERMISSION",
                    Uri.parse("package:" + context.getPackageName())));
        } else {
            Handler handler = new Handler();
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    context.startService(new Intent(context, FloatingModMenuService.class));
                }
            }, 500);
        }
    }

    //Call toast only without mod menu
    public static void LoadLibOnly(final Context context) {
        new Handler().postDelayed(new Runnable() {
            public void run() {
                NativeToast.makeText(context.getApplicationContext(), 0);
            }
        }, 500);
    }
}
