package cn.itcast.mqttclient;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import cn.itcast.mqttclient.toolsActivity.KeyboardActivity;
import cn.itcast.mqttclient.toolsActivity.MouseActivity;
import cn.itcast.mqttclient.toolsActivity.PCPassword;
import cn.itcast.mqttclient.toolsActivity.TokenActivity;
import cn.itcast.mqttclient.toolsActivity.UploadFileActivity;
import cn.itcast.mqttclient.toolsActivity.UploadTextActivity;

public class ToolsActivity extends AppCompatActivity {

    Button btn_mouse, btn_keyboard, tools_btn_pc_password ,tools_btn_token, tools_btn_upload, tools_btn_upload_file;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tools);

        btn_mouse = (Button) findViewById(R.id.btn_mouse);
        btn_keyboard = (Button) findViewById(R.id.btn_keyboard);
        tools_btn_pc_password = (Button) findViewById(R.id.tools_btn_password);
        tools_btn_token = (Button) findViewById(R.id.tools_btn_token);
        tools_btn_upload = (Button) findViewById(R.id.tools_btn_upload);
        tools_btn_upload_file = (Button) findViewById(R.id.tools_btn_upload_file);

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
        tools_btn_pc_password.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(ToolsActivity.this, PCPassword.class);
                startActivity(intent);
            }
        });
        tools_btn_token.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(ToolsActivity.this, TokenActivity.class);
                startActivity(intent);
            }
        });
        tools_btn_upload.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(ToolsActivity.this, UploadTextActivity.class);
                startActivity(intent);
            }
        });
        tools_btn_upload_file.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(ToolsActivity.this, UploadFileActivity.class);
                startActivity(intent);
            }
        });
    }
}