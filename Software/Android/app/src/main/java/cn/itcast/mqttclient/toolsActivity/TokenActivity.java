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
import android.widget.RadioButton;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import cn.itcast.mqttclient.R;

public class TokenActivity extends AppCompatActivity {
    Button token_btn_save, token_btn_cancel;
    RadioButton token_rb_token_open, token_rb_token_close;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_token);

        token_btn_save = (Button) findViewById(R.id.token_btn_save);
        token_btn_cancel = (Button) findViewById(R.id.token_btn_cancel);
        token_rb_token_open = (RadioButton) findViewById(R.id.token_rb_token_open);
        token_rb_token_close = (RadioButton) findViewById(R.id.token_rb_token_close);

        token_btn_save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String jsonData = null;
                if(token_rb_token_open.isChecked())
                {
                    jsonData = generateAppRetainedSettings(getWakeupInterval(), getPowerOnOFF(), getPcPassword(), getPcPasswordCtrl(), getPcPasswordWait(), getLedCtrl(),1);
                }
                else if(token_rb_token_close.isChecked())
                {
                    jsonData = generateAppRetainedSettings(getWakeupInterval(), getPowerOnOFF(), getPcPassword(), getPcPasswordCtrl(), getPcPasswordWait(), getLedCtrl(),0);
                }
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
        token_btn_cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });
    }
}