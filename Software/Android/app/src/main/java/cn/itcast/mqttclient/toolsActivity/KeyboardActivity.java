package cn.itcast.mqttclient.toolsActivity;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;

import cn.itcast.mqttclient.R;
import cn.itcast.mqttclient.ble.BLEInterface;

public class KeyboardActivity extends AppCompatActivity {

    private byte modifier;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_keyboard);

        findViewById(R.id.BtnSpace).setOnTouchListener(new SpaceOnTouch());
        findViewById(R.id.BtnEnter).setOnTouchListener(new EnterOnTouch());
        //findViewById(R.id.BtnFn).setOnTouchListener(new FnKeyOnTouch());

        String packageName = getPackageName();
        KeyOnTouch keyOnTouch = new KeyOnTouch();
        for (int i = 1; i < 72; i++) {
            int resId = getResources().getIdentifier("Btn" + i, "id", packageName);
            if (resId != 0) {
                findViewById(resId).setOnTouchListener(keyOnTouch);
            }
        }

        CtrlKeyOnTouch ctrlKeyOnTouch = new CtrlKeyOnTouch();
        for (int i = 1; i < 6; i++) {
            int resId = getResources().getIdentifier("CtrlBtn" + i, "id", packageName);
            if (resId != 0) {
                findViewById(resId).setOnTouchListener(ctrlKeyOnTouch);
            }
        }
    }

    private class KeyOnTouch implements View.OnTouchListener {
        @SuppressLint("ClickableViewAccessibility")
        @Override
        public boolean onTouch(View v, MotionEvent e) {
            if (e.getAction() == MotionEvent.ACTION_DOWN)
            {
                byte key = (byte) Integer.parseInt(v.getTag().toString(), 16);
                System.out.println((byte) Integer.parseInt(v.getTag().toString(), 16));
                BLEInterface.cmdKeyboardFunc(modifier,key);
            }
            if(e.getAction() == MotionEvent.ACTION_UP)
            {

            }
            return false;
        }
    }

    private class CtrlKeyOnTouch implements View.OnTouchListener {
        @SuppressLint("ClickableViewAccessibility")
        @Override
        public boolean onTouch(View v, MotionEvent e) {
            modifier = (byte) Integer.parseInt(v.getTag().toString(), 16);
            System.out.println((byte) Integer.parseInt(v.getTag().toString(), 16));

            if (e.getAction() == MotionEvent.ACTION_DOWN)
            {

            }
            if(e.getAction() == MotionEvent.ACTION_UP)
            {
                modifier = 0;
                BLEInterface.cmdKeyboardFunc(modifier,(byte)0);
            }
            return false;
        }
    }

    private class SpaceOnTouch implements View.OnTouchListener {
        @SuppressLint("ClickableViewAccessibility")
        @Override
        public boolean onTouch(View v, MotionEvent e) {
            if (e.getAction() == MotionEvent.ACTION_DOWN)
            {
                BLEInterface.cmdKeyboardFunc((byte)0,(byte)0x2C);
            }
            return false;
        }
    }

    private class EnterOnTouch implements View.OnTouchListener {
        @SuppressLint("ClickableViewAccessibility")
        @Override
        public boolean onTouch(View v, MotionEvent e) {
            if (e.getAction() == MotionEvent.ACTION_DOWN)
            {
                BLEInterface.cmdKeyboardFunc((byte)0,(byte)0x28);
            }
            return false;
        }
    }
}



