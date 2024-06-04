package cn.itcast.mqttclient.settingsActivity;

import static cn.itcast.mqttclient.SystemConfig.getMqttAddr;
import static cn.itcast.mqttclient.SystemConfig.getMqttPassword;
import static cn.itcast.mqttclient.SystemConfig.getMqttUserName;
import static cn.itcast.mqttclient.SystemConfig.setMqttAddr;
import static cn.itcast.mqttclient.SystemConfig.setMqttPassword;
import static cn.itcast.mqttclient.SystemConfig.setMqttUserName;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.espressif.iot.esptouch.IEsptouchResult;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.util.List;

import cn.itcast.mqttclient.MainActivity;
import cn.itcast.mqttclient.OnMessageCallback;
import cn.itcast.mqttclient.R;
import cn.itcast.mqttclient.ble.BLEInterface;

public class ServerCfgActivity extends AppCompatActivity {
    private EditText et_mqtt_addr,et_username,et_password;
    private Button btn_server_save, btn_server_save_to_sparrow;
    String addr = "ssl://";
    String username;
    String password;
    private int serverCfgCounter = 0;
    ServerConfigThread thread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_server_cfg);

        thread= new ServerConfigThread();

        initView();
        initEvent();
    }
    private void initView()
    {
        et_mqtt_addr = (EditText) findViewById(R.id.mqtt_addr);
        et_username = (EditText) findViewById(R.id.username);
        et_password = (EditText) findViewById(R.id.password);

        btn_server_save = (Button) findViewById(R.id.btn_server_save);
        btn_server_save_to_sparrow = (Button) findViewById(R.id.btn_server_save_to_sparrow);

        et_mqtt_addr.setText(getMqttAddr(ServerCfgActivity.this).substring(6));
        et_username.setText(getMqttUserName(ServerCfgActivity.this));
        et_password.setText(getMqttPassword(ServerCfgActivity.this));
    }

    private void initEvent()
    {

        btn_server_save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                addr += et_mqtt_addr.getText();
                username = String.valueOf(et_username.getText());
                password = String.valueOf(et_password.getText());
                setMqttAddr(ServerCfgActivity.this, addr);
                setMqttUserName(ServerCfgActivity.this, username);
                setMqttPassword(ServerCfgActivity.this, password);
                Toast.makeText(ServerCfgActivity.this, "保存成功",Toast.LENGTH_SHORT).show();

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
            }
        });
        btn_server_save_to_sparrow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                String addr = "ssl://";
//                String username;
//                String password;

                addr += et_mqtt_addr.getText();
                username = String.valueOf(et_username.getText());
                password = String.valueOf(et_password.getText());
                serverCfgCounter = 1;

                thread.start();
            }
        });

    }

    public void saveSettings(Context context)
    {
        setMqttAddr(context,addr);
        setMqttUserName(context,username);
        setMqttPassword(context,password);
    }

    class ServerConfigThread extends Thread{

        public void run() {
            while (true) {
                switch (serverCfgCounter) {
                    case 1: {
                        BLEInterface.cmdSetServerAddr(addr.getBytes(), (byte) addr.length());
                        serverCfgCounter++;
                        break;
                    }
                    case 2: {
                        BLEInterface.cmdSetServerUsername(username.getBytes(), (byte) username.length());
                        serverCfgCounter++;
                        break;
                    }
                    case 3: {
                        BLEInterface.cmdSetServerPassword(password.getBytes(), (byte) password.length());
                        serverCfgCounter = 0;
                        interrupt();
                        break;
                    }
                }

                try {
                    sleep(200);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}