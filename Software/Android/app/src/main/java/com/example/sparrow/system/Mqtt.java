package com.example.sparrow.system;


import static com.example.sparrow.system.JsonHandler.generateAppDisretainedSettings;
import static com.example.sparrow.system.JsonHandler.generateAppRetainedSettings;
import static com.example.sparrow.system.JsonHandler.generateNotification;
import static com.example.sparrow.system.JsonHandler.getLedCtrl;
import static com.example.sparrow.system.JsonHandler.getPcPassword;
import static com.example.sparrow.system.JsonHandler.getPcPasswordCtrl;
import static com.example.sparrow.system.JsonHandler.getPcPasswordWait;
import static com.example.sparrow.system.JsonHandler.getPowerOnOFF;
import static com.example.sparrow.system.JsonHandler.getSaveAngleFlag;
import static com.example.sparrow.system.JsonHandler.getToken;
import static com.example.sparrow.system.JsonHandler.getTokenCtrl;
import static com.example.sparrow.system.JsonHandler.getTurnAngle;
import static com.example.sparrow.system.JsonHandler.getWakeupInterval;

import android.content.Context;

import com.example.sparrow.MainActivity;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class Mqtt {
    public static MqttClient client = null;

    static Context context;

    public static String mqttServer;//ssl://j1aa1aff.ala.cn-hangzhou.emqxsl.cn:8883";
    public static String userName = "test";
    public static String password = "asd13579";   //用户密码

    /* MQTT 主题 */
    public static String MQTT_TOPIC_APP_RETAINED_SETTINGS = "/settings/app_config/retained";
    public static String MQTT_TOPIC_APP_DISRETAINED_SETTINGS = "/settings/app_config/disretained";
    public static String MQTT_TOPIC_DEVICE_RETAINED_SETTINGS  = "/settings/device_config/retained";
    public static String MQTT_TOPIC_DEVICE_DISRETAINED_SETTINGS = "/settings/device_config/disretained";
    public static String MQTT_TOPIC_DEVICE_RETAINED_STATE = "/state/device_config/retained";
    public static String MQTT_TOPIC_DEVICE_DISRETAINED_STATE = "/state/device_config/disretained";
    public static String MQTT_TOPIC_DEVICE_RETAINED_STATISTICS = "/statistics/device_config/retained";
    public static String MQTT_TOPIC_DEVICE_DISRETAINED_STATISTICS = "/statistics/device_config/disretained";
    public static String MQTT_TOPIC_APP_NOTIFICATION = "/notification";

    public static void connectServer(Context context)
    {
        String clientId = "Controller";
        MemoryPersistence persistence = new MemoryPersistence();

        mqttServer = SystemConfig.getMqttAddr(context);
        userName = SystemConfig.getMqttUserName(context);
        password = SystemConfig.getMqttPassword(context);


        if (mqttServer == null || mqttServer.equals("ssl://")) {
//            mqttServer = "ssl://j1aa1aff.ala.cn-hangzhou.emqxsl.cn:8883";
//            SystemConfig.setMqttAddr(context, mqttServer);
            return;
        }
        if (userName == null) {
//            userName = "test";
//            SystemConfig.setMqttUserName(context, userName);
            return;
        }
        if (password == null) {
//            password = "asd13579";
//            SystemConfig.setMqttPassword(context, password);
            return;
        }

        try {
            client = new MqttClient(mqttServer, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setUserName(userName);
            connOpts.setPassword(password.toCharArray());
            connOpts.setCleanSession(true);
            // 设置回调
            client.setCallback(new OnMessageCallback());
            // 建立连接
            System.out.println("Connecting to broker: " + mqttServer);
            try {
                client.connect(connOpts);
//                Toast.makeText(MainActivity.this,"服务器连接成功",Toast.LENGTH_SHORT).show();
            } catch (MqttException e) {
//                Toast.makeText(MainActivity.this,"服务器连接失败",Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }
            //订阅唤醒间隔主题
            try {
                client.subscribe(MQTT_TOPIC_APP_RETAINED_SETTINGS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅唤醒时长主题
            try {
                client.subscribe(MQTT_TOPIC_APP_DISRETAINED_SETTINGS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅电源控制主题
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_RETAINED_SETTINGS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅连接状态主题
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_DISRETAINED_SETTINGS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅电源状态主题
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_RETAINED_STATE);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_DISRETAINED_STATE);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_RETAINED_STATISTICS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_DISRETAINED_STATISTICS);

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
    }

    public static void mqttSendAppRetainedSettings()
    {
        String jsonData = generateAppRetainedSettings(getWakeupInterval(), getPowerOnOFF(), getPcPassword(), getPcPasswordCtrl(), getPcPasswordWait(), getLedCtrl(), getToken());

        MqttMessage message = new MqttMessage(jsonData.getBytes());
        message.setQos(0);
        message.setRetained(true);
        try {
            client.publish(MQTT_TOPIC_APP_RETAINED_SETTINGS, message);
            System.out.println("Message published");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public static void mqttSendAppDisretainedSettings()
    {
        String jsonData = generateAppDisretainedSettings(getTurnAngle(), getSaveAngleFlag(), getTokenCtrl());

        MqttMessage message = new MqttMessage(jsonData.getBytes());
        message.setQos(0);
        message.setRetained(false);
        try {
            client.publish(MQTT_TOPIC_APP_DISRETAINED_SETTINGS, message);
            System.out.println("Message published");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public static void mqttSendNotification(String nonification)
    {
        // 过滤中文标点
        String regex = "[\\u3002|\\uff1f|\\uff01|\\uff0c|\\u3001|\\uff1b|\\uff1a|\\u201c|\\u201d|\\u2018|\\u2019|\\uff08|\\uff09|\\u300a|\\u300b|\\u3008|\\u3009|\\u3010|\\u3011|\\u300e|\\u300f|\\u300c|\\u300d|\\ufe43|\\ufe44|\\u3014|\\u3015|\\u2026|\\u2014|\\uff5e|\\ufe4f|\\uffe5]";

        String jsonData = generateNotification(nonification.replaceAll(regex,""));

        MqttMessage message = new MqttMessage(jsonData.getBytes());
        message.setQos(0);
        message.setRetained(false);
        try {
            client.publish(MQTT_TOPIC_APP_NOTIFICATION, message);
            System.out.println("Message published");
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
}
