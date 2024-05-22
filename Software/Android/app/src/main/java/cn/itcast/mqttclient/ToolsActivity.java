package cn.itcast.mqttclient;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class ToolsActivity extends AppCompatActivity {

    Button btn_mouse,btn_keyboard;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tools);

        btn_mouse = (Button) findViewById(R.id.btn_mouse);
        btn_keyboard = (Button) findViewById(R.id.btn_keyboard);

        btn_mouse.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(ToolsActivity.this, MouseActivity.class);
                startActivity(intent);
            }
        });
        btn_keyboard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(ToolsActivity.this, KeyboardActivity.class);
                startActivity(intent);
            }
        });
    }
}