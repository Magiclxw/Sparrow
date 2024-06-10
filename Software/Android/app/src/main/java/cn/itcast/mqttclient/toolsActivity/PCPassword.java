package cn.itcast.mqttclient.toolsActivity;

import static cn.itcast.mqttclient.MainActivity.MQTT_TOPIC_APP_RETAINED_SETTINGS;
import static cn.itcast.mqttclient.MainActivity.client;
import static cn.itcast.mqttclient.util.JsonHandler.generateAppRetainedSettings;
import static cn.itcast.mqttclient.util.JsonHandler.getLedCtrl;
import static cn.itcast.mqttclient.util.JsonHandler.getPcPassword;
import static cn.itcast.mqttclient.util.JsonHandler.getPcPasswordCtrl;
import static cn.itcast.mqttclient.util.JsonHandler.getPcPasswordWait;
import static cn.itcast.mqttclient.util.JsonHandler.getPowerOnOFF;
import static cn.itcast.mqttclient.util.JsonHandler.getWakeupInterval;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.NumberPicker;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import cn.itcast.mqttclient.R;

public class PCPassword extends AppCompatActivity {
    NumberPicker np_password_wait_time;
    Button btn_open_password, btn_close_password;
    EditText et_password;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pc_password);

        np_password_wait_time = (NumberPicker) findViewById(R.id.pp_np_password_wait_time);
        btn_open_password = (Button) findViewById(R.id.pp_btn_password_open);
        btn_close_password = (Button) findViewById(R.id.pp_btn_password_close);
        et_password = (EditText) findViewById(R.id.pp_et_password);

        np_password_wait_time.setMaxValue(600);
        np_password_wait_time.setMinValue(0);

        btn_open_password.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String password = String.valueOf(et_password.getText());
                int waitTime = np_password_wait_time.getValue();
                String jsonData = generateAppRetainedSettings(getWakeupInterval(), getPowerOnOFF(), password, 1, waitTime, getLedCtrl());

                MqttMessage message = new MqttMessage(jsonData.getBytes());
                message.setQos(1);
                try {
                    client.publish(MQTT_TOPIC_APP_RETAINED_SETTINGS, message);
                    System.out.println("Message published");
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });

        btn_close_password.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String password = String.valueOf(et_password.getText());
                int waitTime = np_password_wait_time.getValue();
                String jsonData = generateAppRetainedSettings(getWakeupInterval(), getPowerOnOFF(), password, 0, waitTime, getLedCtrl());

                MqttMessage message = new MqttMessage(jsonData.getBytes());
                message.setQos(1);
                try {
                    client.publish(MQTT_TOPIC_APP_RETAINED_SETTINGS, message);
                    System.out.println("Message published");
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });
    }

}