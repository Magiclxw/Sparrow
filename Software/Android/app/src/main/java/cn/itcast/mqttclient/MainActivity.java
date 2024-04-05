  package cn.itcast.mqttclient;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentTransaction;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.NumberPicker;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.io.InputStream;
import java.text.DateFormat;
import java.util.Date;

import cn.itcast.mqttclient.bottomnavigation.MainPage;
import cn.itcast.mqttclient.bottomnavigation.MultiFunc;
import cn.itcast.mqttclient.bottomnavigation.Settings;


public class MainActivity extends AppCompatActivity{
    static String mqttweb="ssl://j1aa1aff.ala.cn-hangzhou.emqxsl.cn:8883";
    private String password = "asd13579";   //用户密码

    /* 主题 */
    private static String tp_set_wakeup_interval = "/settings/wakeup_interval";
    private static String tp_set_wakeup_duration = "/settings/wakeup_duration";
    private static String tp_set_power_on_off  = "/settings/power_on_off";
    private static String tp_sta_connect_status = "/state/connect_status";
    private static String tp_sta_power = "/state/power";
    private static String tp_cfg_angle = "/config/angle";
    private static String tp_cfg_save_angle = "/config/save_angle";

    private Button saveConfig,history,apply,settings;

    static TextView connectState,powerState,powerValue;
    private static RadioGroup power_contral;
    private static NumberPicker numberPicker_wake_hour;
    private static NumberPicker numberPicker_wake_minute;
    private static NumberPicker numberPicker_wakeup_duragion;
    private EditText editText;
    private Context context;
    String content,sendmessage;

    static MqttClient client = null;

    public static Context mContext;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mContext=getApplicationContext();
        //按键
        saveConfig = (Button) findViewById(R.id.save_config);
        history = (Button) findViewById(R.id.history);
        settings = (Button)findViewById(R.id.btn_settings);
        apply = (Button)findViewById(R.id.apply);

        //数字选择器
        numberPicker_wake_hour = (NumberPicker)findViewById(R.id.wakeup_interval_hour);
        numberPicker_wake_hour.setMinValue(0);
        numberPicker_wake_hour.setMaxValue(100);
        //numberPicker_wake_hour.setWrapSelectorWheel(false);
        numberPicker_wake_minute = (NumberPicker)findViewById(R.id.wakeup_interval_minute);
        numberPicker_wake_minute.setMinValue(0);
        numberPicker_wake_minute.setMaxValue(59);
        //numberPicker_wake_minute.setWrapSelectorWheel(false);
        numberPicker_wakeup_duragion = (NumberPicker)findViewById(R.id.wakeup_duration_minute);
        numberPicker_wakeup_duragion.setMinValue(1);
        numberPicker_wakeup_duragion.setMaxValue(59);
        //numberPicker_wakeup_duragion.setWrapSelectorWheel(false);
        //文本
        connectState = (TextView)findViewById(R.id.connect_state);
        powerState = (TextView)findViewById(R.id.power_state);
        powerValue = (TextView)findViewById(R.id.power_value);

        //单选按钮
        power_contral = (RadioGroup)findViewById(R.id.rd_power_ctrl);

        new Thread(new UpdateState()).start();

        String broker = mqttweb;
        String clientId = "Controller";
        MemoryPersistence persistence = new MemoryPersistence();
        try {
            client = new MqttClient(broker, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setUserName("test");
            connOpts.setPassword(password.toCharArray());
            connOpts.setCleanSession(true);
            // 设置回调
            client.setCallback(new OnMessageCallback());
            // 建立连接
            System.out.println("Connecting to broker: " + broker);
            try {
                client.connect(connOpts);
            } catch (MqttException e) {
                Toast.makeText(MainActivity.this,"服务器连接失败",Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }
            //订阅唤醒间隔主题
            try {
                client.subscribe(tp_set_wakeup_interval);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅唤醒时长主题
            try {
                client.subscribe(tp_set_wakeup_duration);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅电源控制主题
            try {
                client.subscribe(tp_set_power_on_off);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅连接状态主题
            try {
                client.subscribe(tp_sta_connect_status);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅电源状态主题
            try {
                client.subscribe(tp_sta_power);

            } catch (MqttException e) {
                e.printStackTrace();
            }


        } catch (MqttException me) {
            System.out.println("reason " + me.getReasonCode());
            System.out.println("msg " + me.getMessage());
            System.out.println("loc " + me.getLocalizedMessage());
            System.out.println("cause " + me.getCause());
            System.out.println("excep " + me);
            me.printStackTrace();
        }
        saveConfig.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int qos = 1;
                String wake_up_interval = String.valueOf(numberPicker_wake_hour.getValue()*60 + numberPicker_wake_minute.getValue());
                String wake_up_duration = String.valueOf(numberPicker_wakeup_duragion.getValue());
                String power_ctrl = String.valueOf(power_contral.getCheckedRadioButtonId());

                MqttMessage message = new MqttMessage(wake_up_interval.getBytes());
                message.setQos(qos);
                try {
                    client.publish(tp_set_wakeup_interval, message);
                    System.out.println("Message published");
                } catch (MqttException e) {
                    e.printStackTrace();
                }
                message.setPayload(wake_up_duration.getBytes());
                try {
                    client.publish(tp_set_wakeup_duration, message);
                    System.out.println("Message published");
                } catch (MqttException e) {
                    e.printStackTrace();
                }

                message.setPayload(power_ctrl.getBytes());
                try {
                    client.publish(tp_set_power_on_off, message);
                    System.out.println("Message published");
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });
        history.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, History.class);
                startActivity(intent);
            }
        });
        apply.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, ApplyJson.class);
                startActivity(intent);
            }
        });
        settings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this,SettingsActivity.class);
                startActivity(intent);
            }
        });

    }
    public static Handler handler=new Handler(){
        @SuppressLint("HandlerLeak")
        public void handleMessage(Message message){
            Bundle bundle=message.getData();

            String interval = bundle.getString(tp_set_wakeup_interval);
            String duration = bundle.getString(tp_set_wakeup_duration);
            String power_ctrl = bundle.getString(tp_set_power_on_off);
            String connect_status = bundle.getString(tp_sta_connect_status);
            String power_state = bundle.getString(tp_sta_power);
            String angle = bundle.getString(tp_cfg_angle);
            String save_angle = bundle.getString(tp_cfg_save_angle);

            if(interval != null){
                int rec_interval = Integer.parseInt(interval);
                numberPicker_wake_hour.setValue(rec_interval/60);   //小时
                numberPicker_wake_minute.setValue(rec_interval%60); //分钟
            }
            if(duration != null)
            {
                int rec_duration = Integer.parseInt(duration);
                numberPicker_wakeup_duragion.setValue(rec_duration);
            }
            if(power_ctrl != null)
            {
                int rec_id = Integer.parseInt(power_ctrl);
                power_contral.check(rec_id);
            }
            if(connect_status != null)
            {
                if(connect_status.equals("0"))
                {
                    connectState.setText("未连接");
                }
                else if(connect_status.equals("1"))
                {
                    connectState.setText("已连接");
                }
            }
            if(power_state != null)
            {
                if(power_state.equals("0"))
                {
                    powerState.setText("无状态");
                }
                else if(power_state.equals("1"))
                {
                    powerState.setText("USB供电");
                }
                else if(power_state.equals("2"))
                {
                    powerState.setText("电池供电");
                }
                else if(power_state.equals("3"))
                {
                    powerState.setText("电压过低");
                }
                else if(power_state.equals("4"))
                {
                    powerState.setText("异常");
                }
            }
        }
    };
    public static void alarm(){
        AlertDialog dialog;
        AlertDialog.Builder builder=new AlertDialog.Builder(MainActivity.mContext)
                .setTitle("通知")
                .setIcon(R.mipmap.cycle)
                .setMessage("收到呼叫")
                .setPositiveButton("确定", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });
        dialog=builder.create();
        dialog.show();
    }

    //设置舵机旋转角度
    public static void TurnAngle(int angle)
    {
        if(angle > 90 || angle < 0) return;

        int qos = 1;

        String turnAngle = String.valueOf(angle);

        MqttMessage message = new MqttMessage(turnAngle.getBytes());
        message.setQos(qos);
        try {
            client.publish(tp_cfg_angle, message);
            System.out.println("Message published");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
    //保存舵机旋转角度
    public static void SaveAngle()
    {
        int qos = 1;

        String saveFlag = "1";

        MqttMessage message = new MqttMessage(saveFlag.getBytes());
        message.setQos(qos);
        try {
            client.publish(tp_cfg_save_angle, message);
            System.out.println("Message published");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    int quitFlag=0;

    public void onBackPressed(){
        if(quitFlag==1){
            MainActivity.this.finish();
        }
        if(quitFlag==0) {
            Toast.makeText(MainActivity.this, "再次返回退出", Toast.LENGTH_LONG).show();
            quitFlag++;
        }
    }
}
