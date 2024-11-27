package com.example.sparrow;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

import com.example.sparrow.tools.NotificationActivity;
import com.example.sparrow.dialog.TokenCtrlDialog;
import com.example.sparrow.tools.KeyboardActivity;
import com.example.sparrow.tools.MouseActivity;
import com.example.sparrow.tools.UploadFileActivity;
import com.example.sparrow.tools.UploadTextActivity;


public class ToolsActivity extends AppCompatActivity {

    Button btn_mouse, btn_keyboard ,tools_btn_token, tools_btn_upload,tools_btn_notification, btn_upload_text, tools_btn_upload_file;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tools);

        btn_mouse = (Button) findViewById(R.id.btn_mouse);
        btn_keyboard = (Button) findViewById(R.id.btn_keyboard);
        tools_btn_token = (Button) findViewById(R.id.btn_token);
        tools_btn_upload = (Button) findViewById(R.id.tools_btn_notification);
        tools_btn_notification = (Button) findViewById(R.id.tools_btn_notification);
        btn_upload_text = (Button) findViewById(R.id.btn_upload_text);
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

        tools_btn_token.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                final TokenCtrlDialog dialog = new TokenCtrlDialog(ToolsActivity.this);
                dialog.show();
            }
        });

        tools_btn_notification.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(ToolsActivity.this, NotificationActivity.class);
                startActivity(intent);
            }
        });

        btn_upload_text.setOnClickListener(new View.OnClickListener() {
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