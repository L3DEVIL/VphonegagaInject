package uk.lgl.modmenu;

import android.app.Activity;
import android.os.Bundle;

import uk.lgl.loadlib.LoadLib;


public class MainActivity extends Activity {
    boolean Switch = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        StaticActivity.Start(this);

        if (Switch){ //keep this code to force compile it. If it's unused, it won't compile LoadLib file
            LoadLib.Start(this);
        }
    }
}
