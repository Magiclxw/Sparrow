package com.example.sparrow;

import static com.example.sparrow.system.JsonHandler.getLedCtrl;
import static com.example.sparrow.system.JsonHandler.setLedCtrl;
import static com.example.sparrow.system.Mqtt.mqttSendAppRetainedSettings;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import com.example.sparrow.dialog.ServoConfigDialog;
import com.example.sparrow.settings.ServerCfgActivity;
import com.example.sparrow.settings.SmartCfgActivity;


public class SettingsActivity extends AppCompatActivity {
    private Button btn_server_settings,btn_servo_cfg, btn_smart_cfg, btn_led_ctrl;
    private NumberPicker turn_angle;
    private int ledCtrlIndex = 0;

    @SuppressLint("CutPasteId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        btn_server_settings = (Button) findViewById(R.id.btn_server_settings);
        btn_servo_cfg = (Button) findViewById(R.id.btn_servo_cfg);
        btn_smart_cfg = (Button) findViewById(R.id.btn_smart_cfg);
        btn_led_ctrl = (Button) findViewById(R.id.btn_led_ctrl);

        btn_server_settings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(SettingsActivity.this, ServerCfgActivity.class);
                startActivity(intent);
            }
        });
        btn_servo_cfg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                final ServoConfigDialog dialog = new ServoConfigDialog(SettingsActivity.this);
                dialog.show();
            }
        });
        btn_smart_cfg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(SettingsActivity.this, SmartCfgActivity.class);
                startActivity(intent);
            }
        });
        btn_led_ctrl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                AlertDialog dialog;
                AlertDialog.Builder builder = new AlertDialog.Builder(SettingsActivity.this)
                        .setTitle("LED控制")
                        .setSingleChoiceItems(new String[]{"打开","关闭"}, getLedCtrl(), new DialogInterface.OnClickListener(){
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                // 获取被选项
                                ledCtrlIndex = i;
                            }
                        })
                        .setPositiveButton("确定", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                setLedCtrl(ledCtrlIndex);   // 0：打开，1：关闭
                                mqttSendAppRetainedSettings();
                            }
                        })
                        .setNegativeButton("取消", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                dialogInterface.dismiss();
                            }
                        });
                dialog = builder.create();
                dialog.show();
            }
        });

    }

}
