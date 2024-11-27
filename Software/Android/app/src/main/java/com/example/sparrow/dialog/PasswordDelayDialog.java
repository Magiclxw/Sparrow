package com.example.sparrow.dialog;

import static com.example.sparrow.system.JsonHandler.setPcPasswordWait;
import static com.example.sparrow.system.Mqtt.mqttSendAppRetainedSettings;

import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;

import com.example.sparrow.R;

public class PasswordDelayDialog extends AlertDialog {
    NumberPicker np_password_delay;
    Button btn_password_delay_confirm;

    public PasswordDelayDialog(Context context) {
        super(context);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dialog_password_delay);

        initView();
        initEvent();
    }

    private void initView()
    {
        np_password_delay = (NumberPicker) findViewById(R.id.np_password_delay);
        btn_password_delay_confirm = (Button) findViewById(R.id.btn_password_delay_confirm);

        np_password_delay.setMaxValue(600);
        np_password_delay.setMinValue(10);
    }

    private void initEvent()
    {
        btn_password_delay_confirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                setPcPasswordWait(np_password_delay.getValue());
                mqttSendAppRetainedSettings();
            }
        });
    }
}
