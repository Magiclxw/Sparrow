package cn.itcast.mqttclient.dialog;

import static cn.itcast.mqttclient.MainActivity.MQTT_TOPIC_APP_DISRETAINED_SETTINGS;
import static cn.itcast.mqttclient.MainActivity.MQTT_TOPIC_APP_RETAINED_SETTINGS;
import static cn.itcast.mqttclient.MainActivity.client;

import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;


public class ServoCfgDialog extends AlertDialog {

    private NumberPicker np_angle;
    private Button btn_center_angle,btn_set_angle,btn_save_angle;

    public ServoCfgDialog(Context context) {
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
        btn_save_angle = (Button) findViewById(R.id.btn_save_angle);
    }

    private void initEvent()
    {
        btn_center_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TurnAngle(0);
            }
        });

        btn_set_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TurnAngle(np_angle.getValue()-90); //-90~90
            }
        });

        btn_save_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SaveAngle();
                dismiss();
            }
        });
    }

    //设置舵机旋转角度
    public static void TurnAngle(int angle)
    {
        if(angle > 90 || angle < -90) return;

        int qos = 1;

        String turnAngle = String.valueOf(angle);

        MqttMessage message = new MqttMessage(turnAngle.getBytes());
        message.setQos(qos);
        try {
            client.publish(MQTT_TOPIC_APP_RETAINED_SETTINGS, message);
            System.out.println("Message published");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
    //保存舵机旋转角度
    public static void SaveAngle()
    {
        int qos = 1;

        String saveFlag = "1";

        MqttMessage message = new MqttMessage(saveFlag.getBytes());
        message.setQos(qos);
        try {
            client.publish(MQTT_TOPIC_APP_DISRETAINED_SETTINGS, message);
            System.out.println("Message published");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
}
