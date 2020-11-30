package uk.lgl.modmenu;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

//TODO
//Write up android logcat on readme

public class Preferences {
    private static SharedPreferences.Editor editor;
    public static Context context;
    public static boolean isSoundEnabled = true, savePref = false, animation = true, expanded = false;

    public static native void Changes(Context context, int feature, int value, boolean bool, String str);

    public static void changeFeatureInt(String feature, int featureNum, int value) {
        Changes(context, featureNum, value, false, feature);
        editor.putInt(String.valueOf(featureNum), value).apply();
    }

    public static void changeFeatureBoolean(String feature, int featureNum, boolean value) {
        if (featureNum == 1000)
            isSoundEnabled = value;
        if (featureNum == 1001)
            animation = value;
        if (featureNum == 1002)
            expanded = value;
        if (featureNum == 9998)
            savePref = value;
        Changes(context, featureNum, 0, value, feature);
        editor.putBoolean(String.valueOf(featureNum), value).apply();
    }

    //TODO: changeFeatureString

    public static int loadPrefInt(String featureName, int featureNum) {
        if (savePref) {
            SharedPreferences preferences = context.getSharedPreferences("mod_menu", 0);
            editor = preferences.edit();
            int i = preferences.getInt(String.valueOf(featureNum), 0);
            Changes(context, featureNum, i, false, featureName);
            return i;
        }
        return -1;
    }

    public static boolean loadPrefBoolean(String featureName, int featureNum) {
        SharedPreferences preferences = context.getSharedPreferences("mod_menu", 0);
        if (featureNum >= 9998) {
            savePref = preferences.getBoolean(String.valueOf(featureNum), false);
        }
        if (savePref || featureNum >= 1000) {
            editor = preferences.edit();
            if (featureNum == 1000 && !preferences.contains("1000"))
                return true;
            if (featureNum == 1001 && !preferences.contains("1001"))
                return true;
            boolean bool = preferences.getBoolean(String.valueOf(featureNum), false);
            Changes(context, featureNum, 0, bool, featureName);
            return bool;
        }
        return false;
    }
}