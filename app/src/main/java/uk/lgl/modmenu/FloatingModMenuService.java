package uk.lgl.modmenu;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.os.IBinder;
import android.support.v4.view.ViewCompat;
import android.text.Html;
import android.util.Base64;
import android.util.Log;
import android.util.TypedValue;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;

import static uk.lgl.modmenu.StaticActivity.cacheDir;

public class FloatingModMenuService extends Service {
    private MediaPlayer FXPlayer;
    public View mFloatingView;
    private Button close;
    private Button kill;
    private LinearLayout mButtonPanel;
    public RelativeLayout mCollapsed;
    public LinearLayout mExpanded;
    private RelativeLayout mRootContainer;
    public WindowManager mWindowManager;
    public WindowManager.LayoutParams params;
    private LinearLayout patches;
    private FrameLayout rootFrame;
    private ImageView startimage;
    private LinearLayout view1;
    private LinearLayout view2;

    private static final String TAG = "Mod Menu";

    private boolean soundEnabled = true;

    //initialize methods from the native library
    public static native String Toast();

    private native String Title();

    private native String Heading();

    private native String Icon();

    private native int IconSize();

    public native void changeSeekBar(int i, int i2);

    public native void changeToggle(int i);

    private native String[] getFeatureList();

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    //Override our Start Command so the Service doesnt try to recreate itself when the App is closed
    public int onStartCommand(Intent intent, int i, int i2) {
        return Service.START_NOT_STICKY;
    }

    //When this Class is called the code in this function will be executed
    @Override
    public void onCreate() {
        super.onCreate();
        //A little message for the user when he opens the app
        //Toast.makeText(this, Toast(), Toast.LENGTH_LONG).show();
        //Init Lib

        // When you change the lib name, change also on Android.mk file
        // Both must have same name
        System.loadLibrary("LGLTeam");

        initFloating();

        final Handler handler = new Handler();
        handler.post(new Runnable() {
            public void run() {
                FloatingModMenuService.this.Thread();
                handler.postDelayed(this, 1000);
            }
        });
    }

    //Here we write the code for our Menu
    private void initFloating() {
        this.rootFrame = new FrameLayout(getBaseContext()); // Global markup
        this.mRootContainer = new RelativeLayout(getBaseContext()); // Markup on which two markups of the icon and the menu itself will be placed
        this.mCollapsed = new RelativeLayout(getBaseContext()); // Markup of the icon (when the menu is minimized)
        this.mExpanded = new LinearLayout(getBaseContext()); // Menu markup (when the menu is expanded)
        this.view1 = new LinearLayout(getBaseContext());
        this.patches = new LinearLayout(getBaseContext());
        this.view2 = new LinearLayout(getBaseContext());
        this.mButtonPanel = new LinearLayout(getBaseContext()); // Layout of option buttons (when the menu is expanded)

        RelativeLayout relativeLayout = new RelativeLayout(this);
        relativeLayout.setLayoutParams(new RelativeLayout.LayoutParams(-2, -1));
        relativeLayout.setPadding(10, 10, 10, 10);
        relativeLayout.setVerticalGravity(16);

        this.kill = new Button(this);
        this.kill.setBackgroundColor(Color.parseColor("#1C2A35"));
        this.kill.setText("HIDE");
        this.kill.setTextColor(Color.parseColor("#82CAFD"));

        RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(-2, -2);
        layoutParams.addRule(11);

        this.close = new Button(this);
        this.close.setBackgroundColor(Color.parseColor("#1C2A35"));
        this.close.setText("CLOSE");
        this.close.setTextColor(Color.parseColor("#82CAFD"));
        this.close.setLayoutParams(layoutParams);

        relativeLayout.addView(this.kill);
        relativeLayout.addView(this.close);

        this.rootFrame.setLayoutParams(new FrameLayout.LayoutParams(-1, -1));
        this.mRootContainer.setLayoutParams(new FrameLayout.LayoutParams(-2, -2));
        this.mCollapsed.setLayoutParams(new RelativeLayout.LayoutParams(-2, -2));
        this.mCollapsed.setVisibility(View.VISIBLE);
        this.startimage = new ImageView(getBaseContext());
        this.startimage.setLayoutParams(new RelativeLayout.LayoutParams(-2, -2));
        int applyDimension = (int) TypedValue.applyDimension(1, (float) IconSize(), getResources().getDisplayMetrics());
        this.startimage.getLayoutParams().height = applyDimension;
        this.startimage.getLayoutParams().width = applyDimension;
        this.startimage.requestLayout();
        this.startimage.setScaleType(ImageView.ScaleType.FIT_XY);
        byte[] decode = Base64.decode(Icon(), 0);
        this.startimage.setImageBitmap(BitmapFactory.decodeByteArray(decode, 0, decode.length));
        this.startimage.setImageAlpha(200);
        ((ViewGroup.MarginLayoutParams) this.startimage.getLayoutParams()).topMargin = convertDipToPixels(10);

        this.mExpanded.setVisibility(View.GONE);
        this.mExpanded.setBackgroundColor(Color.parseColor("#1C2A35"));
        this.mExpanded.setAlpha(0.95f);
        this.mExpanded.setGravity(17);
        this.mExpanded.setOrientation(LinearLayout.VERTICAL);
        this.mExpanded.setPadding(5, 0, 5, 0);
        this.mExpanded.setLayoutParams(new LinearLayout.LayoutParams(-1, -2));

        ScrollView scrollView = new ScrollView(getBaseContext());
        scrollView.setLayoutParams(new LinearLayout.LayoutParams(-1, dp(190)));
        scrollView.setBackgroundColor(Color.parseColor("#FF000000"));

        this.view1.setLayoutParams(new LinearLayout.LayoutParams(-1, 5));
        this.view1.setBackgroundColor(Color.parseColor("#1C2A35"));
        this.patches.setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
        this.patches.setOrientation(LinearLayout.VERTICAL);
        this.view2.setLayoutParams(new LinearLayout.LayoutParams(-1, 5));
        this.view2.setBackgroundColor(Color.parseColor("#1C2A35"));
        this.view2.setPadding(0, 0, 0, 10);
        this.mButtonPanel.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));

        //Title text
        TextView textView = new TextView(getBaseContext());
        textView.setText(Title());
        textView.setTextColor(Color.parseColor("#82CAFD"));
        textView.setTypeface(Typeface.DEFAULT_BOLD);
        textView.setTextSize(20.0f);
        textView.setPadding(10, 25, 10, 5);
        LinearLayout.LayoutParams layoutParams2 = new LinearLayout.LayoutParams(-2, -2);
        layoutParams2.gravity = 17;
        textView.setLayoutParams(layoutParams2);

        //Heading text
        TextView textView2 = new TextView(getBaseContext());
        textView2.setText(Html.fromHtml(Heading()));
        textView2.setTextColor(Color.parseColor("#82CAFD"));
        textView2.setTypeface(Typeface.DEFAULT_BOLD);
        textView2.setTextSize(10.0f);
        textView2.setPadding(10, 5, 10, 10);

        LinearLayout.LayoutParams layoutParams3 = new LinearLayout.LayoutParams(-2, -2);
        layoutParams3.gravity = 17;
        textView2.setLayoutParams(layoutParams3);
        new LinearLayout.LayoutParams(-1, dp(25)).topMargin = dp(2);
        this.rootFrame.addView(this.mRootContainer);
        this.mRootContainer.addView(this.mCollapsed);
        this.mRootContainer.addView(this.mExpanded);
        this.mCollapsed.addView(this.startimage);
        this.mExpanded.addView(textView);
        this.mExpanded.addView(textView2);
        this.mExpanded.addView(this.view1);
        this.mExpanded.addView(scrollView);
        scrollView.addView(this.patches);
        this.mExpanded.addView(this.view2);
        this.mExpanded.addView(relativeLayout);
        this.mFloatingView = this.rootFrame;
        if (Build.VERSION.SDK_INT >= 26) {
            this.params = new WindowManager.LayoutParams(-2, -2, 2038, 8, -3);
        } else {
            this.params = new WindowManager.LayoutParams(-2, -2, 2002, 8, -3);
        }
        WindowManager.LayoutParams layoutParams4 = this.params;
        layoutParams4.gravity = 51;
        layoutParams4.x = 0;
        layoutParams4.y = 100;
        this.mWindowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        this.mWindowManager.addView(this.mFloatingView, this.params);
        RelativeLayout relativeLayout2 = this.mCollapsed;
        LinearLayout linearLayout = this.mExpanded;
        this.mFloatingView.setOnTouchListener(onTouchListener());
        this.startimage.setOnTouchListener(onTouchListener());
        initMenuButton(relativeLayout2, linearLayout);
        CreateMenuList();
    }

    private View.OnTouchListener onTouchListener() {
        return new View.OnTouchListener() {
            final View collapsedView = FloatingModMenuService.this.mCollapsed;
            final View expandedView = FloatingModMenuService.this.mExpanded;
            private float initialTouchX;
            private float initialTouchY;
            private int initialX;
            private int initialY;

            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        this.initialX = FloatingModMenuService.this.params.x;
                        this.initialY = FloatingModMenuService.this.params.y;
                        this.initialTouchX = motionEvent.getRawX();
                        this.initialTouchY = motionEvent.getRawY();
                        return true;
                    case MotionEvent.ACTION_UP:
                        int rawX = (int) (motionEvent.getRawX() - this.initialTouchX);
                        int rawY = (int) (motionEvent.getRawY() - this.initialTouchY);

                        //The check for Xdiff <10 && YDiff< 10 because sometime elements moves a little while clicking.
                        //So that is click event.
                        if (rawX < 10 && rawY < 10 && FloatingModMenuService.this.isViewCollapsed()) {
                            //When user clicks on the image view of the collapsed layout,
                            //visibility of the collapsed layout will be changed to "View.GONE"
                            //and expanded view will become visible.
                            this.collapsedView.setVisibility(View.GONE);
                            this.expandedView.setVisibility(View.VISIBLE);
                            playSound(Uri.fromFile(new File(cacheDir + "OpenMenu.ogg")));
                            //Toast.makeText(FloatingModMenuService.this, Html.fromHtml(Toast()), Toast.LENGTH_SHORT).show();
                        }
                        return true;
                    case MotionEvent.ACTION_MOVE:
                        //Calculate the X and Y coordinates of the view.
                        FloatingModMenuService.this.params.x = this.initialX + ((int) (motionEvent.getRawX() - this.initialTouchX));
                        FloatingModMenuService.this.params.y = this.initialY + ((int) (motionEvent.getRawY() - this.initialTouchY));

                        //Update the layout with new X & Y coordinate
                        FloatingModMenuService.this.mWindowManager.updateViewLayout(FloatingModMenuService.this.mFloatingView, FloatingModMenuService.this.params);
                        return true;
                    default:
                        return false;
                }
            }
        };
    }

    //Initialize event handlers for buttons, etc.
    private void initMenuButton(final View view2, final View view3) {
        this.startimage.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view2.setVisibility(View.GONE);
                view3.setVisibility(View.VISIBLE);
            }
        });
        this.kill.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                FloatingModMenuService.this.stopSelf();
                playSound(Uri.fromFile(new File(cacheDir + "Select.ogg")));
            }
        });
        this.close.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view2.setVisibility(View.VISIBLE);
                view3.setVisibility(View.GONE);
                playSound(Uri.fromFile(new File(cacheDir + "Back.ogg")));
                //Log.i("LGL", "Close");
            }
        });
    }

    public int ii;

    private void CreateMenuList() {
        String[] listFT = getFeatureList();
        for (int i = 0; i < listFT.length; i++) {
            ii = i;
            //Log.i(TAG, String.valueOf(i));
            String str = listFT[i];
            if (str.contains("Toggle_")) {
                final int finalI = i;
                addSwitch(str.replace("Toggle_", ""), i, new SW() {
                    public void OnWrite(boolean z) {
                        FloatingModMenuService.this.changeToggle(finalI);
                    }
                });
            } else if (str.contains("SeekBar_")) {
                final int finalI = i;
                String[] split = str.split("_");
                addSeekBar(split[1], i, Integer.parseInt(split[2]), Integer.parseInt(split[3]), new SB() {
                    public void OnWrite(int i) {
                        FloatingModMenuService.this.changeSeekBar(finalI, i);
                    }
                });
            } else {
                final int finalI = i;
                addSwitch(str, i, new SW() {
                    public void OnWrite(boolean z) {
                        FloatingModMenuService.this.changeToggle(finalI);
                    }
                });
            }
        }
    }

    //Just a little function to draw a toggle so we dont have to do this all the time when we want to draw one
    private void addSwitch(String str, int i, final SW sw) {
        // Log.i("LGL", "Add switch");
        Switch switchR = new Switch(this);
        if (i % 2 == 0) {
            switchR.setBackgroundColor(Color.parseColor("#171E24"));
        } else {
            switchR.setBackgroundColor(Color.parseColor("#171E24"));
        }
        switchR.setText(Html.fromHtml("<font face='monospace'><b>" + str + "</b></font>"));
        switchR.setTextColor(Color.parseColor("#DEEDF6"));
        switchR.setPadding(10, 5, 0, 5);
        switchR.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton compoundButton, boolean z) {
                playSound(Uri.fromFile(new File(cacheDir + "SliderSwitch.ogg")));
                sw.OnWrite(z);
            }
        });
        this.patches.addView(switchR);
    }

    private void addSeekBar(String str, int i, int i2, int i3, SB sb) {
        int i4 = i2;
        LinearLayout linearLayout = new LinearLayout(this);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(-1, -1);
        linearLayout.setPadding(10, 5, 0, 5);
        linearLayout.setOrientation(LinearLayout.VERTICAL);
        linearLayout.setGravity(17);
        linearLayout.setLayoutParams(layoutParams);
        if (i % 2 == 0) {
            linearLayout.setBackgroundColor(Color.parseColor("#171E24"));
        } else {
            linearLayout.setBackgroundColor(Color.parseColor("#171E24"));
        }
        final TextView textView = new TextView(this);
        StringBuilder sb2 = new StringBuilder();
        sb2.append("<font face='monospace'><b>");
        final String str2 = str;
        sb2.append(str);
        sb2.append(": <font color='#41c300'>");
        sb2.append(i2);
        sb2.append("</b></font>");
        textView.setText(Html.fromHtml(sb2.toString()));
        textView.setTextColor(Color.parseColor("#DEEDF6"));
        SeekBar seekBar = new SeekBar(this);
        seekBar.setPadding(25, 10, 35, 10);
        seekBar.setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
        seekBar.setMax(i3);
        seekBar.setProgress(i2);
        final int i5 = i2;
        final SeekBar seekBar2 = seekBar;
        final SB sb3 = sb;
        final TextView textView2 = textView;
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }

            public void onProgressChanged(SeekBar seekBar, int i, boolean z) {
                playSound(Uri.fromFile(new File(cacheDir + "SliderSwitch.ogg")));
                int i2 = i5;
                if (i < i2) {
                    seekBar2.setProgress(i2);
                    sb3.OnWrite(i5);
                    TextView textView = textView2;
                    textView.setText(Html.fromHtml("<font face='monospace'><b>" + str2 + ": <font color='#41c300'>" + i5 + "</b></font>"));
                    return;
                }
                sb3.OnWrite(i);
                textView.setText(Html.fromHtml("<font face='monospace'><b>" + str2 + ": <font color='#41c300'>" + i + "</b></font>"));
            }
        });
        linearLayout.addView(textView);
        linearLayout.addView(seekBar);
        this.patches.addView(linearLayout);
    }

    boolean delayed;

    public void playSound(Uri uri) {
        if (soundEnabled) {
            if (!delayed) {
                delayed = true;
                if (FXPlayer != null) {
                    FXPlayer.stop();
                    FXPlayer.release();
                }
                FXPlayer = MediaPlayer.create(this, uri);
                if (FXPlayer != null)
                    FXPlayer.start();

                Handler handler = new Handler();
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        delayed = false;
                    }
                }, 90);
            }
        }
    }

    public boolean isViewCollapsed() {
        return this.mFloatingView == null || this.mCollapsed.getVisibility() == View.VISIBLE;
    }

    //For our image a little converter
    private int convertDipToPixels(int i) {
        return (int) ((((float) i) * getResources().getDisplayMetrics().density) + 0.5f);
    }

    private int dp(int i) {
        return (int) TypedValue.applyDimension(1, (float) i, getResources().getDisplayMetrics());
    }

    //Destroy our View
    public void onDestroy() {
        super.onDestroy();
        View view = this.mFloatingView;
        if (view != null) {
            this.mWindowManager.removeView(view);
        }
    }

    //Check if we are still in the game. If now our Menu and Menu button will dissapear
    private boolean isNotInGame() {
        RunningAppProcessInfo runningAppProcessInfo = new RunningAppProcessInfo();
        ActivityManager.getMyMemoryState(runningAppProcessInfo);
        return runningAppProcessInfo.importance != 100;
    }

    //Same as above so it wont crash in the background and therefore use alot of Battery life
    public void onTaskRemoved(Intent intent) {
        stopSelf();
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        super.onTaskRemoved(intent);
    }

    /* access modifiers changed from: private */
    public void Thread() {
        if (this.mFloatingView == null) {
            return;
        }
        if (isNotInGame()) {
            this.mFloatingView.setVisibility(View.INVISIBLE);
        } else {
            this.mFloatingView.setVisibility(View.VISIBLE);
        }
    }

    private interface SB {
        void OnWrite(int i);
    }

    private interface SW {
        void OnWrite(boolean z);
    }
}
