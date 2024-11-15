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
    private final static String JSON_KEY_LAST_WAKUP_TIME = "last_wakeup_time";
    private final static String JSON_KEY_LAST_SLEEP_TIME = "last_sleep_time";
    private final static String JSON_KEY_NEXT_WAKEUP_TIME = "next_wakeup_time";
    private final static String JSON_KEY_NEXT_SLEEP_TIME = "next_sleep_time";
    private final static String JSON_KEY_DEVICE_STATE = "on_line_state";

    private final static String JSON_KEY_POWER_ON_COUNT = "power_on_count";
    private final static String JSON_KEY_POWER_OFF_COUNT = "power_off_count";
    private final static String JSON_KEY_WAKE_UP_COUNT = "wake_up_count";

    private final static String JSON_KEY_VOLTAGE = "voltage";

    private final static String JSON_KEY_NOTIFICATION = "notification";

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
    private static long lastPowerOnTime = 0;
    private static long lastPowerOffTime = 0;
    private static long nextPowerOnTime = 0;
    private static long nextPowerOffTime = 0;
    private static long lastWakeupTime = 0;
    private static long lastSleepTime = 0;
    private static long nextWakeupTime = 0;
    private static long nextSleepTime = 0;
    private static int deviceState = 0;

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

    public static void analysisDevRetainedState(String jsonData)
    {
        try {
            JSONObject jsonObject = new JSONObject(jsonData);

            power = jsonObject.getInt(JSON_KEY_POWER);
            lastPowerOnTime = jsonObject.getLong(JSON_KEY_LAST_POWER_ON_TIME);
            lastPowerOffTime = jsonObject.getLong(JSON_KEY_LAST_POWER_OFF_TIME);
            nextPowerOnTime = jsonObject.getLong(JSON_KEY_NEXT_POWER_ON_TIME);
            nextPowerOffTime = jsonObject.getLong(JSON_KEY_NEXT_POWER_OFF_TIME);
            lastWakeupTime = jsonObject.getLong(JSON_KEY_LAST_WAKUP_TIME);
            lastSleepTime = jsonObject.getLong(JSON_KEY_LAST_SLEEP_TIME);
            nextWakeupTime = jsonObject.getLong(JSON_KEY_NEXT_WAKEUP_TIME);
            nextSleepTime = jsonObject.getLong(JSON_KEY_NEXT_SLEEP_TIME);
            deviceState = jsonObject.getInt(JSON_KEY_DEVICE_STATE);


            System.out.println(power+"+"+lastPowerOnTime+"+"+lastPowerOffTime+"+"+nextPowerOnTime+"+"+nextPowerOffTime+"+"+lastWakeupTime+"+"+lastSleepTime+"+"+nextWakeupTime+"+"+nextSleepTime);

        } catch (JSONException e) {
            System.out.println("device retained data analyse error");
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

    public static String generateAppDisretainedSettings(int turnAngle, int saveFlag)
    {
        String jsonString = "null";
        try {
            JSONObject jsonObject = new JSONObject();

            jsonObject.put(JSON_KEY_TURN_ANGLE, turnAngle);
            jsonObject.put(JSON_KEY_SAVE_ANGLE, saveFlag);

            jsonString = jsonObject.toString();

        } catch (JSONException e) {
            e.printStackTrace();
        }

        return jsonString;
    }

    public static String generateNotification(String notification)
    {
        String jsonString = "null";
        try {
            JSONObject jsonObject = new JSONObject();

            jsonObject.put(JSON_KEY_NOTIFICATION, notification);

            jsonString = jsonObject.toString();

        } catch (JSONException e) {
            e.printStackTrace();
        }

        return jsonString;
    }

/** APP保留设置数据 **/
    public static int getWakeupInterval()
    {
        return wakeupInterval;
    }

    public static void setWakeupInterval(int interval)
    {
        wakeupInterval = interval;
    }

    public static int getPowerOnOFF()
    {
        return powerOnOFF;
    }

    public static void setPowerOnOff(int power)
    {
        powerOnOFF = power;
    }

    public static String getPcPassword()
    {
        return pcPassword;
    }

    public static void setPcPassword(String password)
    {
        pcPassword = password;
    }

    public static int getPcPasswordCtrl()
    {
        return pcPasswordCtrl;
    }

    public static void setPcPasswordCtrl(int ctrl)
    {
        pcPasswordCtrl = ctrl;
    }

    public static int getPcPasswordWait()
    {
        return pcPasswordWait;
    }

    public static void setPcPasswordWait(int time)
    {
        pcPasswordWait = time;
    }

    public static int getLedCtrl()
    {
        return ledCtrl;
    }

    public static void setLedCtrl(int ctrl)
    {
        ledCtrl = ctrl;
    }

    public static int getToken()
    {
        return toolsToken;
    }

    public static void setToken(int token)
    {
        toolsToken = token;
    }

/** APP非保留设置数据 **/
    public static int getTurnAngle()
    {
        return turnAngle;
    }

    public static void setTurnAngle(int angle)
    {
        turnAngle = angle;
    }

    public static int getSaveAngleFlag()
    {
        return saveAngle;
    }

    public static void setSaveAngleFlag(int flag)
    {
        saveAngle = flag;
    }

/** Device 保留状态数据 **/
    public static int getPowerState()
    {
        return power;
    }

    public static long getLastPowerOnTime()
    {
        return lastPowerOnTime;
    }

    public static long getLastPowerOffTime()
    {
        return  lastPowerOffTime;
    }

    public static long getNextPowerOnTime()
    {
        return  nextPowerOnTime;
    }

    public static long getNextPowerOffTime()
    {
        return  nextPowerOffTime;
    }

    public static long getLastWakeupTime()
    {
        return  lastWakeupTime;
    }

    public static long getLastSleepTime()
    {
        return  lastSleepTime;
    }

    public static long getNextWakeupTime()
    {
        return  nextWakeupTime;
    }

    public static long getNextSleepTime()
    {
        return  nextSleepTime;
    }

    public static int getDeviceState()
    {
        return deviceState;
    }
}
