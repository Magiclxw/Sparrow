package com.example.sparrow.dialog;

import static com.example.sparrow.system.JsonHandler.setPcPasswordWait;
import static com.example.sparrow.system.JsonHandler.setSaveAngleFlag;
import static com.example.sparrow.system.JsonHandler.setToken;
import static com.example.sparrow.system.JsonHandler.setTokenCtrl;
import static com.example.sparrow.system.Mqtt.mqttSendAppDisretainedSettings;
import static com.example.sparrow.system.Mqtt.mqttSendAppRetainedSettings;

import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;

import com.example.sparrow.R;

public class TokenCtrlDialog extends AlertDialog {
    Button btn_lock_screen, btn_unlock_screen;

    public TokenCtrlDialog(Context context) {
        super(context);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dialog_token_ctrl);

        initView();
        initEvent();
    }

    private void initView()
    {
        btn_lock_screen = (Button) findViewById(R.id.btn_lock_screen);
        btn_unlock_screen = (Button) findViewById(R.id.btn_unlock_screen);
    }

    private void initEvent()
    {
        btn_lock_screen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // 关闭舵机控制
                setSaveAngleFlag(0);
                setTokenCtrl(2);
                mqttSendAppDisretainedSettings();
            }
        });

        btn_unlock_screen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // 关闭舵机控制
                setSaveAngleFlag(0);
                setTokenCtrl(1);
                mqttSendAppDisretainedSettings();
            }
        });
    }
}

