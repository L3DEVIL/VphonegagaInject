package uk.lgl.modmenu;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.os.Process;
import android.provider.Settings;
import android.util.Base64;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;

public class StaticActivity {

    private static final String TAG = "Mod Menu";
    public static String cacheDir;

    public static void Start(final Context context) {

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !Settings.canDrawOverlays(context)) {
            AlertDialog alertDialog = new AlertDialog.Builder(context, 1)
                    .setTitle("No overlay permission")
                    .setMessage("Overlay permission is required in order to display the mod menu on top of the screen. Do you want to open the settings?")
                    .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            context.startActivity(new Intent("android.settings.action.MANAGE_OVERLAY_PERMISSION",
                                    Uri.parse("package:" + context.getPackageName())));
                            Process.killProcess(Process.myPid());
                        }
                    })
                    .setNegativeButton("No", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    }).setCancelable(false)
                    .create();
            alertDialog.show();
        } else {
            // When you change the lib name, change also on Android.mk file
            // Both must have same name
            System.loadLibrary("MyLibName");

            Handler handler = new Handler();
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    context.startService(new Intent(context, FloatingModMenuService.class));
                }
            }, 2000);

            cacheDir = context.getCacheDir().getPath() + "/";

            writeToFile("OpenMenu.ogg", Sounds.OpenMenu());
            writeToFile("Back.ogg", Sounds.Back());
            writeToFile("Select.ogg", Sounds.Select());
            writeToFile("SliderIncrease.ogg", Sounds.SliderIncrease());
            writeToFile("SliderDecrease.ogg", Sounds.SliderDecrease());
            writeToFile("On.ogg", Sounds.On());
            writeToFile("Off.ogg", Sounds.Off());
        }
    }

    private static void writeToFile(String name, String base64) {
        File file = new File(cacheDir + name);
        try {
            if (!file.exists()) {
                file.createNewFile();
            }
            FileOutputStream fos = new FileOutputStream(file);
            byte[] decode = Base64.decode(base64, 0);
            fos.write(decode);
            fos.close();
        } catch (Exception e) {
            Log.e(TAG, e.getMessage());
        }
    }
}
