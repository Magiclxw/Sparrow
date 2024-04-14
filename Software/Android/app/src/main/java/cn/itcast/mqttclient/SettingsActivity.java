package cn.itcast.mqttclient;

import static cn.itcast.mqttclient.SystemConfig.getMqttAddr;
import static cn.itcast.mqttclient.SystemConfig.getMqttPassword;
import static cn.itcast.mqttclient.SystemConfig.getMqttUserName;
import static cn.itcast.mqttclient.SystemConfig.setMqttAddr;
import static cn.itcast.mqttclient.SystemConfig.setMqttPassword;
import static cn.itcast.mqttclient.SystemConfig.setMqttUserName;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.NumberPicker;

import androidx.appcompat.app.AppCompatActivity;

import cn.itcast.mqttclient.dialog.ServerDialog;
import cn.itcast.mqttclient.dialog.ServoCfgDialog;

public class SettingsActivity extends AppCompatActivity {
    private Button back_to_zero,set_turn_angle,save_turn_angle,btn_server_settings,btn_servo_cfg;
    private NumberPicker turn_angle;

    @SuppressLint("CutPasteId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        btn_server_settings = (Button) findViewById(R.id.btn_server_settings);
        btn_servo_cfg = (Button) findViewById(R.id.btn_servo_cfg);

        btn_server_settings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                final ServerDialog dialog = new ServerDialog(SettingsActivity.this);
                dialog.show();
            }
        });
        btn_servo_cfg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                final ServoCfgDialog dialog = new ServoCfgDialog(SettingsActivity.this);
                dialog.show();
            }
        });
    }


}
