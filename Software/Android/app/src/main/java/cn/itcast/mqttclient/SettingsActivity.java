package cn.itcast.mqttclient;

import static cn.itcast.mqttclient.MainActivity.MQTT_TOPIC_APP_RETAINED_SETTINGS;
import static cn.itcast.mqttclient.MainActivity.client;
import static cn.itcast.mqttclient.util.JsonHandler.generateAppRetainedSettings;
import static cn.itcast.mqttclient.util.JsonHandler.getLedCtrl;
import static cn.itcast.mqttclient.util.JsonHandler.getPcPassword;
import static cn.itcast.mqttclient.util.JsonHandler.getPcPasswordCtrl;
import static cn.itcast.mqttclient.util.JsonHandler.getPcPasswordWait;
import static cn.itcast.mqttclient.util.JsonHandler.getPowerOnOFF;
import static cn.itcast.mqttclient.util.JsonHandler.getToken;
import static cn.itcast.mqttclient.util.JsonHandler.getWakeupInterval;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import cn.itcast.mqttclient.dialog.ServoCfgDialog;
import cn.itcast.mqttclient.settingsActivity.ServerCfgActivity;
import cn.itcast.mqttclient.settingsActivity.SmartCfgActivity;

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
                final ServoCfgDialog dialog = new ServoCfgDialog(SettingsActivity.this);
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
                                ledCtrlIndex = i;
                            }
                        })
                        .setPositiveButton("确定", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                String jsonData = generateAppRetainedSettings(getWakeupInterval(), getPowerOnOFF(), getPcPassword(), getPcPasswordCtrl(), getPcPasswordWait(), ledCtrlIndex, getToken());
                                MqttMessage message = new MqttMessage(jsonData.getBytes());
                                message.setQos(1);
                                try {
                                    client.publish(MQTT_TOPIC_APP_RETAINED_SETTINGS, message);
                                    System.out.println("Message published");
                                } catch (MqttException e) {
                                    e.printStackTrace();
                                }

                                dialogInterface.dismiss();
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
