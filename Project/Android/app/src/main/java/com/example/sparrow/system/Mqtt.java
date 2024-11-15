package com.example.sparrow.system;


import static com.example.sparrow.system.JsonHandler.generateAppRetainedSettings;
import static com.example.sparrow.system.JsonHandler.generateNotification;
import static com.example.sparrow.system.JsonHandler.getLedCtrl;
import static com.example.sparrow.system.JsonHandler.getPcPassword;
import static com.example.sparrow.system.JsonHandler.getPcPasswordCtrl;
import static com.example.sparrow.system.JsonHandler.getPcPasswordWait;
import static com.example.sparrow.system.JsonHandler.getPowerOnOFF;
import static com.example.sparrow.system.JsonHandler.getToken;
import static com.example.sparrow.system.JsonHandler.getWakeupInterval;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class Mqtt {
    public static MqttClient client = null;

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

    public static void connectServer(String broker, String userName, String password)
    {
        String clientId = "Controller";
        MemoryPersistence persistence = new MemoryPersistence();

        try {
            client = new MqttClient(broker, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setUserName(userName);
            connOpts.setPassword(password.toCharArray());
            connOpts.setCleanSession(true);
            // 设置回调
            client.setCallback(new OnMessageCallback());
            // 建立连接
            System.out.println("Connecting to broker: " + broker);
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

    public static void mqttSendNotification(String nonification)
    {
        String jsonData = generateNotification(nonification);

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
