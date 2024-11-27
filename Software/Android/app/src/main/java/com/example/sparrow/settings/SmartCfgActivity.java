package com.example.sparrow.settings;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Looper;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.espressif.iot.esptouch.EsptouchTask;
import com.espressif.iot.esptouch.IEsptouchListener;
import com.espressif.iot.esptouch.IEsptouchResult;
import com.example.sparrow.R;

import java.util.List;

public class SmartCfgActivity extends AppCompatActivity {

    int frequency = 0;
    Button btn_smart_cfg;
    EditText et_ssid,et_bssid,et_password,et_frequency;

    EsptouchTask task;

    byte[] apSsid = {}; // Set AP's SSID
    byte[] apBssid = {}; // Set AP's BSSID
    byte[] apPassword = {}; // Set AP's password

    @SuppressLint("ResourceAsColor")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_smart_cfg);

        et_ssid = (EditText) findViewById(R.id.et_ssid);
        et_bssid = (EditText) findViewById(R.id.et_bssid);
        et_password = (EditText) findViewById(R.id.et_password);
        et_frequency = (EditText) findViewById(R.id.et_frequency);

        btn_smart_cfg = (Button) findViewById(R.id.btn_start_smart_cfg);

        et_ssid.setText(getSSID());
        et_bssid.setText(getStrBSSID());
        frequency = getFrequency();

        et_frequency.setText( frequency + "MHz");

        if(frequency > 5000)
        {
            et_frequency.setTextColor(Color.RED);
            et_frequency.append("(请切换为2.4G WIFI)");
        }

        apSsid = getSSID().toString().getBytes();
        apBssid = getBSSID();

        EspTouchThread espTouchThread = new EspTouchThread();

        btn_smart_cfg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                apPassword = et_password.getText().toString().getBytes();

                if (apPassword != null && apPassword.length > 5 && task == null && frequency < 2600)
                {
                    task = new EsptouchTask(apSsid, apBssid, apPassword, SmartCfgActivity.this);

                    task.setPackageBroadcast(true); // if true send broadcast packets, else send multicast packets

                    task.setEsptouchListener(new IEsptouchListener() {
                        @Override
                        public void onEsptouchResultAdded(IEsptouchResult result) {
                            // Result callback
                            if(result.isSuc() == true)
                            {
                                Looper.prepare();
                                Toast.makeText(SmartCfgActivity.this, "配网成功！", Toast.LENGTH_LONG).show();
                                Looper.loop();
                            }
                            else if(result.isCancelled() == true)
                            {
                                Looper.prepare();
                                Toast.makeText(SmartCfgActivity.this, "配网失败", Toast.LENGTH_LONG).show();
                                Looper.loop();
                            }
                        }
                    });
                    espTouchThread.start();
                }
            }
        });

    }

    private String getSSID()
    {
        WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
        if (wm != null) {
            WifiInfo winfo = wm.getConnectionInfo();
            if (winfo != null) {
                String s = winfo.getSSID();
                if (s.length() > 2 && s.charAt(0) == '"' && s.charAt(s.length() - 1) == '"') {
                    return s.substring(1, s.length() - 1);
                }
            }
        }
        return "";
    }

    private String getStrBSSID()
    {
        WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
        if (wm != null) {
            WifiInfo winfo = wm.getConnectionInfo();
            if (winfo != null) {
                String s = winfo.getBSSID();
                if (s.length() > 2) {
                    return s;
                }
            }
        }
        return "";
    }

    private byte[] getBSSID()
    {
        byte[] bssidBytes = new byte[6];

        WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
        if (wm != null) {
            WifiInfo winfo = wm.getConnectionInfo();
            if (winfo != null) {
                String s = winfo.getBSSID();
                if (s.length() > 2) {
                    String[] bssidParts = s.split(":");
                    for (int i = 0; i < bssidParts.length; i++) {
                        bssidBytes[i] = (byte) Integer.parseInt(bssidParts[i], 16);
                    }
                    return bssidBytes;
                }
            }
        }
        return bssidBytes;
    }

    private int getFrequency()
    {
        int frequency = 0;

        WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
        if (wm != null) {
            WifiInfo winfo = wm.getConnectionInfo();
            if (winfo != null)
            {
                frequency = winfo.getFrequency();
            }
        }
        return frequency;
    }

    class EspTouchThread extends Thread{
        public void run() {
            int expectResultCount = 1;

            List<IEsptouchResult> results = task.executeForResults(expectResultCount);
            IEsptouchResult first = results.get(0);
            if (first.isCancelled()) {
                // User cancel the task
                return;
            }
            if (first.isSuc()) {
                // EspTouch successfully
                // 关闭task
                task.interrupt();
                // 关闭线程
                interrupt();
            }
        }
    }
}