package com.example.sparrow.settings;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import androidx.appcompat.app.AppCompatActivity;

import com.example.sparrow.R;
import com.example.sparrow.system.SystemConfig;


public class BleNameSetActivity extends AppCompatActivity {
    Button btn_set_ble_name;
    EditText et_set_ble_name;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ble_name_set);

        btn_set_ble_name = (Button) findViewById(R.id.btn_set_bel_name);
        et_set_ble_name = (EditText) findViewById(R.id.et_set_ble_name);

        btn_set_ble_name.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String name = String.valueOf(et_set_ble_name.getText());
                SystemConfig.setBleName(BleNameSetActivity.this, name);
            }
        });
    }
}