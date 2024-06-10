package cn.itcast.mqttclient.toolsActivity;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.SystemClock;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import cn.itcast.mqttclient.R;
import cn.itcast.mqttclient.ble.BLEInterface;

public class MouseActivity extends AppCompatActivity {

    public TextView pox,poY;
    private EditText et_keyboard;
    private Button btn_Left,btn_Right;
    public View view_TouchPad;
    private final static long CLICK_INTERVAL = 100;
    private int touchFlag = 0;
    private float startPosX,startPosY,stopPosX,stopPosY;
    long firstClickStart,firstClickStop;
    private int leftPressFlag,rightPressFlag;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mouse);
        pox = (TextView)findViewById(R.id.text);
        poY = (TextView)findViewById(R.id.text2);

        et_keyboard = (EditText) findViewById(R.id.et_keyboard);
        view_TouchPad = (View) findViewById(R.id.view_touchpad);

        btn_Left = (Button) findViewById(R.id.btn_left);
        btn_Right = (Button) findViewById(R.id.btn_right);


        et_keyboard.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                System.out.println("input");
                return false;
            }
        });

        et_keyboard.addTextChangedListener(textWatcher);

        et_keyboard.setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View view, int i, KeyEvent keyEvent) {
                //软键盘删除按键按下
                if (i == 67)
                {
                    BLEInterface.cmdKeyboardFunc((byte)0,BLEInterface.HID_KEY_BACKSPACE);
                }
                return false;
            }
        });

        btn_Left.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                try {
                    switch (motionEvent.getAction())
                    {
                        case MotionEvent.ACTION_DOWN:
                            BLEInterface.cmdMouseClick(BLEInterface.MOUSE_LEFT_BUTTON, BLEInterface.MOUSE_KEY_PRESSED);
                            break;
                        case MotionEvent.ACTION_UP:
                            BLEInterface.cmdMouseClick(BLEInterface.MOUSE_LEFT_BUTTON, BLEInterface.MOUSE_KEY_RELEASED);
                            break;
                    }
                    return true;
                } catch (Exception e) {
                    Log.v("touch", e.toString());
                    return false;
                }
            }
        });

        btn_Right.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                try {
                    switch (motionEvent.getAction())
                    {
                        case MotionEvent.ACTION_DOWN:
                            BLEInterface.cmdMouseClick(BLEInterface.MOUSE_RIGHT_BUTTON, BLEInterface.MOUSE_KEY_PRESSED);
                            break;
                        case MotionEvent.ACTION_UP:
                            BLEInterface.cmdMouseClick(BLEInterface.MOUSE_RIGHT_BUTTON, BLEInterface.MOUSE_KEY_RELEASED);
                            break;
                    }
                    return true;
                } catch (Exception e) {
                    Log.v("touch", e.toString());
                    return false;
                }
            }
        });

        view_TouchPad.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                float x = motionEvent.getX();
                float y = motionEvent.getY();
                try {
                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            touchFlag = 1;
                            pox.setText("" + x);
                            poY.setText("" + y);
                            startPosX = x;
                            startPosY = y;
                            firstClickStart = SystemClock.uptimeMillis();
                            break;
                        case MotionEvent.ACTION_UP:
                            touchFlag = 0;
                            pox.setText("" + x);
                            poY.setText("" + y);
                            stopPosX = x;
                            stopPosY = y;
                            firstClickStop = SystemClock.uptimeMillis();
                            if(firstClickStop - firstClickStart < CLICK_INTERVAL)   //满足单击事件
                            {
                                BLEInterface.cmdMouseClick(BLEInterface.MOUSE_LEFT_BUTTON, BLEInterface.MOUSE_KEY_CLICKED);
                                System.out.println("单击事件");
                            }
                            else
                            {
                                int moveX = (int)(startPosX - stopPosX);
                                int moveY = (int)(startPosY - stopPosX);
                            }
                            break;
                        case MotionEvent.ACTION_MOVE:
                            pox.setText("" + x);
                            poY.setText("" + y);
                            if(SystemClock.uptimeMillis() - firstClickStart > CLICK_INTERVAL)   //满足单击事件
                            {
                                BLEInterface.cmdMouseMove((byte)((x-startPosX)),(byte)((y-startPosY)));
                                startPosX = x;
                                startPosY = y;
                            }
                            break;
                    }
                    return true;
                } catch (Exception e) {
                    Log.v("touch", e.toString());
                    return false;
                }
            }
        });

    }
//    @Override
//    public boolean onTouchEvent(MotionEvent event) {
//
//    }

    private TextWatcher textWatcher = new TextWatcher() {
        @Override
        public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

        }

        @Override
        public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

        }

        @Override
        public void afterTextChanged(Editable editable) {
            String text = et_keyboard.getText().toString();
            if (text.isEmpty() == false)
            {
                BLEInterface.cmdKeyboardInput((byte)text.charAt(0));
                et_keyboard.setText("");
            }
        }
    };
}
