package com.example.sparrow;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

import com.example.sparrow.tools.KeyboardActivity;
import com.example.sparrow.tools.MouseActivity;


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

//        tools_btn_token.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Intent intent = new Intent();
//                intent.setClass(ToolsActivity.this, TokenActivity.class);
//                startActivity(intent);
//            }
//        });

    }
}