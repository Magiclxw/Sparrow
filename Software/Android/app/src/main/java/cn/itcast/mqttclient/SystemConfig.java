package cn.itcast.mqttclient;

import android.content.Context;
import android.content.SharedPreferences;

public class SystemConfig {

    public static void setMqttAddr(Context context, String addr)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = sp.edit();
        edit.putString("addr",addr);
        edit.commit();
    }

    public static String getMqttAddr(Context context)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        return sp.getString("addr",null);
    }

    public static void setMqttUserName(Context context,String userName)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = sp.edit();
        edit.putString("userName",userName);
        edit.commit();
    }

    public static String getMqttUserName(Context context)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        return sp.getString("userName",null);
    }

    public static void setMqttPassword(Context context,String password)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = sp.edit();
        edit.putString("password",password);
        edit.commit();
    }

    public static String getMqttPassword(Context context)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        return sp.getString("password",null);
    }

    public static void setBleServiceUUID(Context context,String uuid)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = sp.edit();
        edit.putString("service_uuid",uuid);
        edit.commit();
    }

    public static String getBleServiceUUID(Context context)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        return sp.getString("service_uuid",null);
    }
    public static void setBleReadUUID(Context context,String uuid)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = sp.edit();
        edit.putString("read_uuid",uuid);
        edit.commit();
    }

    public static String getBleReadUUID(Context context)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        return sp.getString("read_uuid",null);
    }

    public static void setBleWriteUUID(Context context,String uuid)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = sp.edit();
        edit.putString("write_uuid",uuid);
        edit.commit();
    }

    public static String getBleWriteUUID(Context context)
    {
        SharedPreferences sp = context.getSharedPreferences("data",Context.MODE_PRIVATE);
        return sp.getString("write_uuid",null);
    }

}
