package cn.itcast.mqttclient;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;

import androidx.appcompat.app.AppCompatActivity;

import cn.itcast.mqttclient.dialog.ServerDialog;
import cn.itcast.mqttclient.dialog.ServoCfgDialog;
import cn.itcast.mqttclient.settingsActivity.SmartCfgActivity;

public class SettingsActivity extends AppCompatActivity {
    private Button btn_server_settings,btn_servo_cfg, btn_smart_cfg;
    private NumberPicker turn_angle;

    @SuppressLint("CutPasteId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        btn_server_settings = (Button) findViewById(R.id.btn_server_settings);
        btn_servo_cfg = (Button) findViewById(R.id.btn_servo_cfg);
        btn_smart_cfg = (Button) findViewById(R.id.btn_smart_cfg);

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
        btn_smart_cfg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(SettingsActivity.this, SmartCfgActivity.class);
                startActivity(intent);
            }
        });


    }

}
