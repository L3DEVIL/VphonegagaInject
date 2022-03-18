package uk.lgl;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Typeface;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

//import androidx.annotation.RequiresApi;
import java.util.Date;

import uk.lgl.modmenu.FloatingModMenuService;

//import com.thunder.injector.L3DEVILFloater;

//import uk.lgl.modmenu.FloatingModMenuService;

public class ESPView extends View implements Runnable {
    int FPS = 60;
    Paint mFilledPaint;
    Paint mStrokePaint;
    Paint mTextPaint;
    Thread mThread;
    long sleepTime;
    Date time;
    int screenWidth;
    int screenHeight;

    public ESPView(Context context) {
        super(context, (AttributeSet) null, 0);
        InitializePaints();
        setFocusableInTouchMode(false);
        setBackgroundColor(0);
        this.time = new Date();
        this.sleepTime = (long) (1000 / this.FPS);
        this.mThread = new Thread(this);
        this.mThread.start();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (canvas != null && getVisibility() == VISIBLE) {
            ClearCanvas(canvas);
            screenWidth = (canvas.getWidth() / 2);
            screenHeight = (canvas.getHeight());

            FloatingModMenuService.DrawOn(this, canvas);
//            Floater.Teste(this, canvas, screenWidth -(screenWidth), screenHeight -(screenHeight));
        }
    }

    @Override
    public void run() {
        android.os.Process.setThreadPriority(10);
        while (this.mThread.isAlive() && !this.mThread.isInterrupted()) {
            try {
                long currentTimeMillis = System.currentTimeMillis();
                postInvalidate();
                Thread.sleep(Math.max(Math.min(0, this.sleepTime - (System.currentTimeMillis() - currentTimeMillis)), this.sleepTime));
            } catch (InterruptedException e) {
                Log.e("OverlayThread", e.getMessage());
            }
        }
    }

    public void InitializePaints() {

        this.mStrokePaint = new Paint();
        this.mStrokePaint.setAntiAlias(true);

        this.mFilledPaint = new Paint();
        this.mFilledPaint.setStyle(Paint.Style.FILL);
        this.mFilledPaint.setAntiAlias(true);

        this.mTextPaint = new Paint();
        this.mTextPaint.setAntiAlias(true);
        this.mTextPaint.setTypeface(Typeface.MONOSPACE);
        this.mTextPaint.setTextAlign(Paint.Align.CENTER);
    }

    public void ClearCanvas(Canvas cvs) {
        cvs.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
    }

    public void DrawLine(Canvas cvs, int a, int r, int g, int b, float lineWidth, float fromX, float fromY, float toX, float toY) {
        mStrokePaint.setColor(Color.rgb(r, g, b));
        mStrokePaint.setAlpha(a);
        mStrokePaint.setStrokeWidth(lineWidth);
        cvs.drawLine(fromX, fromY, toX, toY, mStrokePaint);
    }

    public void DrawText(Canvas cvs, int a, int r, int g, int b, float stroke, String txt, float posX, float posY, float size) {
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        mTextPaint.setStrokeWidth(stroke);
        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(4.0f + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(size + 2.0f);
        else
            mTextPaint.setTextSize(size);

        cvs.drawText(txt, posX, posY, mTextPaint);
    }

    public void DrawCircle(Canvas cvs, int a, int r, int g, int b, float stroke, float posX, float posY, float radius) {
        mStrokePaint.setColor(Color.rgb(r, g, b));
        mStrokePaint.setAlpha(a);
        mStrokePaint.setStrokeWidth(stroke);
        cvs.drawCircle(posX, posY, radius, mStrokePaint);
    }

    public void DrawFilledCircle(Canvas cvs, int a, int r, int g, int b, float posX, float posY, float radius) {
        mFilledPaint.setColor(Color.rgb(r, g, b));
        mFilledPaint.setAlpha(a);
        cvs.drawCircle(posX, posY, radius, mFilledPaint);
    }

    public void DrawRect(Canvas cvs, int a, int r, int g, int b, int stroke, float x, float y, float width, float height) {
        mStrokePaint.setStrokeWidth(stroke);
        mStrokePaint.setColor(Color.rgb(r, g, b));
        mStrokePaint.setAlpha(a);
        cvs.drawRect(x, y, x + width, y + height, mStrokePaint);
    }

    public void DrawFilledRect(Canvas cvs, int a, int r, int g, int b, float x, float y, float width, float height) {
        mFilledPaint.setColor(Color.rgb(r, g, b));
        mFilledPaint.setAlpha(a);
        cvs.drawRect(x, y, x + width, y + height, mFilledPaint);
    }

//    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public void DrawFilledOval(Canvas cvs, int a, int r, int g, int b, float x, float y, float width, float height) {
        mFilledPaint.setColor(Color.rgb(r, g, b));
        mFilledPaint.setAlpha(a);
        cvs.drawOval(x, y, x + width, y + height, mFilledPaint);
    }
}