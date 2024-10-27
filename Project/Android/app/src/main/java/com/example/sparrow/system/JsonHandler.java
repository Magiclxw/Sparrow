package com.example.sparrow.system;

import org.json.JSONException;
import org.json.JSONObject;

public class JsonHandler {
    private final static String JSON_KEY_WAKE_UP_INTERVAL = "wakeup_interval";
    private final static String JSON_KEY_POWER_ON_OFF = "power_on_off";
    private final static String JSON_KEY_PC_PASSWORD = "pc_password";
    private final static String JSON_KEY_PC_PASSWORD_CTRL = "pc_password_ctrl";
    private final static String JSON_KEY_PC_PASSWORD_WAIT = "pc_password_wait";
    private final static String JSON_KEY_LED_CTRL = "led_ctrl";
    private final static String JSON_KEY_TOOLS_TOKEN = "tools_token";


    private final static String JSON_KEY_TURN_ANGLE = "turn_angle";
    private final static String JSON_KEY_SAVE_ANGLE = "save_angle";

    private final static String JSON_KEY_POWER = "power";
    private final static String JSON_KEY_LAST_POWER_ON_TIME = "last_power_on_time";
    private final static String JSON_KEY_LAST_POWER_OFF_TIME = "last_power_off_time";
    private final static String JSON_KEY_NEXT_POWER_ON_TIME = "next_power_on_time";
    private final static String JSON_KEY_NEXT_POWER_OFF_TIME = "next_power_off_time";
    private final static String JSON_KEY_LAST_WAK_UP_TIME = "last_wakeup_time";
    private final static String JSON_KEY_LAST_SLEEP_TIME = "last_sleep_time";
    private final static String JSON_KEY_NEXT_WAKE_UP_TIME = "next_wake_up_time";
    private final static String JSON_KEY_NEXT_SLEEP_TIME = "next_sleep_time";

    private final static String JSON_KEY_POWER_ON_COUNT = "power_on_count";
    private final static String JSON_KEY_POWER_OFF_COUNT = "power_off_count";
    private final static String JSON_KEY_WAKE_UP_COUNT = "wake_up_count";

    private final static String JSON_KEY_VOLTAGE = "voltage";

    private static int wakeupInterval = 0;
    private static int powerOnOFF = 0;
    private static String pcPassword = null;
    private static int pcPasswordCtrl = 0;
    private static int pcPasswordWait = 0;
    private static int ledCtrl = 0;
    private static int toolsToken = 0;

    private static int turnAngle = 0;
    private static int saveAngle = 0;

    private static int power = 0;
    private static int lastPowerOnTime = 0;
    private static int lastPowerOffTime = 0;
    private static int nextPowerOnTime = 0;
    private static int nextPowerOffTime = 0;
    private static int lastWakeupTime = 0;
    private static int lastSleepTime = 0;
    private static int nextWakeupTime = 0;
    private static int nextSleepTime = 0;

    private static int powerOnCount = 0;
    private static int powerOffCount = 0;
    private static int wakeupCount = 0;

    private static int voltage = 0;

    public static void analysisAppRetainedSettings(String jsonData) {
        try {
            JSONObject jsonObject = new JSONObject(jsonData);

            wakeupInterval = jsonObject.getInt(JSON_KEY_WAKE_UP_INTERVAL);
            powerOnOFF = jsonObject.getInt(JSON_KEY_POWER_ON_OFF);
            pcPassword = jsonObject.getString(JSON_KEY_PC_PASSWORD);
            pcPasswordCtrl = jsonObject.getInt(JSON_KEY_PC_PASSWORD_CTRL);
            pcPasswordWait = jsonObject.getInt(JSON_KEY_PC_PASSWORD_WAIT);
            ledCtrl = jsonObject.getInt(JSON_KEY_LED_CTRL);
            toolsToken = jsonObject.getInt(JSON_KEY_TOOLS_TOKEN);

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public static String generateAppRetainedSettings(int wakeupInterval, int powerOnOFF, String pcPassword, int pcPasswordCtrl, int pcPasswordWait, int ledCtrl, int token)
    {
        String jsonString = "null";

        try {
            JSONObject jsonObject = new JSONObject();

            jsonObject.put(JSON_KEY_WAKE_UP_INTERVAL, wakeupInterval);
            jsonObject.put(JSON_KEY_POWER_ON_OFF, powerOnOFF);
            jsonObject.put(JSON_KEY_PC_PASSWORD, pcPassword);
            jsonObject.put(JSON_KEY_PC_PASSWORD_CTRL, pcPasswordCtrl);
            jsonObject.put(JSON_KEY_PC_PASSWORD_WAIT, pcPasswordWait);
            jsonObject.put(JSON_KEY_LED_CTRL, ledCtrl);
            jsonObject.put(JSON_KEY_TOOLS_TOKEN, token);

            jsonString = jsonObject.toString();

        } catch (JSONException e) {
            e.printStackTrace();
        }

        return jsonString;
    }

    public static int getWakeupInterval()
    {
        return wakeupInterval;
    }

    public static int getPowerOnOFF()
    {
        return powerOnOFF;
    }

    public static String getPcPassword()
    {
        return pcPassword;
    }

    public static int getPcPasswordCtrl()
    {
        return pcPasswordCtrl;
    }

    public static int getPcPasswordWait()
    {
        return pcPasswordWait;
    }

    public static int getLedCtrl()
    {
        return ledCtrl;
    }

    public static int getToken()
    {
        return toolsToken;
    }
}
