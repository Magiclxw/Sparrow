package com.example.sparrow.ui.settings.dialog;

import static com.example.sparrow.system.JsonHandler.generateAppDisretainedSettings;
import static com.example.sparrow.system.JsonHandler.getSaveAngleFlag;
import static com.example.sparrow.system.JsonHandler.getTurnAngle;
import static com.example.sparrow.system.Mqtt.MQTT_TOPIC_APP_DISRETAINED_SETTINGS;
import static com.example.sparrow.system.Mqtt.MQTT_TOPIC_APP_RETAINED_SETTINGS;
import static com.example.sparrow.system.Mqtt.client;

import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;

import com.example.sparrow.R;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class ServoConfigDialog extends AlertDialog {

    private NumberPicker np_angle;
    private Button btn_center_angle,btn_set_angle,btn_save_as_idle,btn_save_as_forward,btn_save_as_backword;

    public ServoConfigDialog(Context context) {
        super(context);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dialog_servo_cfg);
        initView();
        initEvent();
    }

    private void initView()
    {
        np_angle = (NumberPicker) findViewById(R.id.np_angle);
        np_angle.setMaxValue(180);
        np_angle.setMinValue(0);

        btn_center_angle = (Button) findViewById(R.id.btn_center_angle);
        btn_set_angle = (Button) findViewById(R.id.btn_set_angle);
        btn_save_as_idle = (Button) findViewById(R.id.btn_save_as_idle);
        btn_save_as_forward = (Button) findViewById(R.id.btn_save_as_forward);
        btn_save_as_backword = (Button) findViewById(R.id.btn_save_as_backword);
    }

    private void initEvent()
    {
        btn_center_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TurnAngle(90);
            }
        });

        btn_set_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TurnAngle(np_angle.getValue());
            }
        });

        btn_save_as_idle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // 保存为空闲时角度
                SaveAngle(1);
                dismiss();
            }
        });

        btn_save_as_forward.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // 保存为正向角度
                SaveAngle(2);
                dismiss();
            }
        });

        btn_save_as_backword.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // 保存为反向角度
                SaveAngle(3);
                dismiss();
            }
        });
    }

    //设置舵机旋转角度
    public static void TurnAngle(int angle)
    {
//        if(angle > 90 || angle < -90) return;

        int qos = 1;

//        String turnAngle = String.valueOf(angle-90);

        String mqttData = generateAppDisretainedSettings(angle-90,getSaveAngleFlag());

        MqttMessage message = new MqttMessage(mqttData.getBytes());
        message.setQos(qos);
        try {
            client.publish(MQTT_TOPIC_APP_DISRETAINED_SETTINGS, message);
            System.out.println("Message published");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
    //保存舵机旋转角度
    public static void SaveAngle(int flag)
    {
        int qos = 1;

        String mqttData = generateAppDisretainedSettings(getTurnAngle(),flag);

        MqttMessage message = new MqttMessage(mqttData.getBytes());
        message.setQos(qos);
        try {
            client.publish(MQTT_TOPIC_APP_DISRETAINED_SETTINGS, message);
            System.out.println("Message published");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
}