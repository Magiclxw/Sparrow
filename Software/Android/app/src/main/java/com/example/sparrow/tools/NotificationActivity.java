package com.example.sparrow.tools;

import static com.example.sparrow.system.Mqtt.mqttSendNotification;

import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import androidx.appcompat.app.AppCompatActivity;

import com.example.sparrow.R;

public class NotificationActivity extends AppCompatActivity {
    EditText et_notification;
    Button btn_send_notification;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dialog_notification);

        et_notification = (EditText) findViewById(R.id.et_notification);
        btn_send_notification = (Button) findViewById(R.id.btn_send_notification);

        btn_send_notification.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String text = String.valueOf(et_notification.getText());
                mqttSendNotification(text);
            }
        });
    }
}
