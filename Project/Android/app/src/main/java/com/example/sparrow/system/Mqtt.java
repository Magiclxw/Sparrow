package com.example.sparrow.system;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
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
}
