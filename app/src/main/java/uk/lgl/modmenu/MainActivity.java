package uk.lgl.modmenu;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;

import uk.lgl.loadlib.LoadLib;

public class MainActivity extends Activity {
    boolean DummySwitch = false;
    public String GameActivity = "com.unity3d.player.UnityPlayerActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //To launch mod menu
        StaticActivity.Start(this);

        //To load lib only
        //LoadLib.Start(this);

        //To launch game activity
        try {
            MainActivity.this.startActivity(new Intent(MainActivity.this, Class.forName(MainActivity.this.GameActivity)));
            //Start service
        } catch (ClassNotFoundException e) {
            Toast.makeText(MainActivity.this, "Error. Game's main activity does not exist", Toast.LENGTH_LONG).show();
            e.printStackTrace();
            return;
        }

        //keep this code to force compile it. If it's unused, it won't compile the LoadLib.java file
        if (DummySwitch){
            LoadLib.Start(this);
        }
    }
}
