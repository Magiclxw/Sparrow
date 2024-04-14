package cn.itcast.mqttclient.dialog;

import static cn.itcast.mqttclient.SystemConfig.getMqttAddr;
import static cn.itcast.mqttclient.SystemConfig.getMqttPassword;
import static cn.itcast.mqttclient.SystemConfig.getMqttUserName;
import static cn.itcast.mqttclient.SystemConfig.setMqttAddr;
import static cn.itcast.mqttclient.SystemConfig.setMqttPassword;
import static cn.itcast.mqttclient.SystemConfig.setMqttUserName;

import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import cn.itcast.mqttclient.MainActivity;
import cn.itcast.mqttclient.OnMessageCallback;
import cn.itcast.mqttclient.R;
import cn.itcast.mqttclient.SettingsActivity;

public class ServerDialog extends AlertDialog {
    private EditText et_mqtt_addr,et_username,et_password;
    private Button btn_server_save,btn_server_cancel;
    String addr = "ssl://";
    String username;
    String password;

    public ServerDialog(Context context) {
        super(context);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dialog_server);
        initView();
        initEvent();
    }

    private void initView()
    {
        et_mqtt_addr = (EditText) findViewById(R.id.mqtt_addr);
        et_username = (EditText) findViewById(R.id.username);
        et_password = (EditText) findViewById(R.id.password);

        btn_server_save = (Button) findViewById(R.id.btn_server_save);
        btn_server_cancel = (Button) findViewById(R.id.btn_server_cancel);

        et_mqtt_addr.setText(getMqttAddr(super.getContext()).substring(6));
        et_username.setText(getMqttUserName(super.getContext()));
        et_password.setText(getMqttPassword(super.getContext()));
    }

    private void initEvent()
    {
        btn_server_save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                addr += et_mqtt_addr.getText();
                username = String.valueOf(et_username.getText());
                password = String.valueOf(et_password.getText());
                setMqttAddr(ServerDialog.super.getContext(),addr);
                setMqttUserName(ServerDialog.super.getContext(),username);
                setMqttPassword(ServerDialog.super.getContext(),password);
                Toast.makeText(ServerDialog.super.getContext(),"保存成功",Toast.LENGTH_SHORT).show();

                System.out.println("连接断开，可以做重连");
                String broker = addr;
                String clientId = "Controller";
                MemoryPersistence persistence = new MemoryPersistence();

                try {
                    MainActivity.client = new MqttClient(broker, clientId, persistence);
                    MqttConnectOptions connOpts = new MqttConnectOptions();
                    connOpts.setUserName(username);
                    connOpts.setPassword(password.toCharArray());
                    connOpts.setCleanSession(true);
                    // 设置回调
                    MainActivity.client.setCallback(new OnMessageCallback());
                    // 建立连接
                    System.out.println("Connecting to broker: " + broker);
                    try {
                        MainActivity.client.connect(connOpts);
                        System.out.println("重连成功");
                    } catch (MqttException e) {
                        e.printStackTrace();
                    }
                } catch (MqttException me) {
                    me.printStackTrace();
                }

                dismiss();
            }
        });

        btn_server_cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                dismiss();
            }
        });
    }

    @Override
    public void show() {
        super.show();
    }

    public void saveSettings(Context context)
    {
        setMqttAddr(context,addr);
        setMqttUserName(context,username);
        setMqttPassword(context,password);
    }
}
