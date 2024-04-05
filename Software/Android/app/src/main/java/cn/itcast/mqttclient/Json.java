package cn.itcast.mqttclient;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Json extends AppCompatActivity {
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.listview);
        Intent intent=getIntent();
        String content=intent.getStringExtra("content");
        try {
            JSONArray jsonArray=new JSONArray(content);
            for(int i=0;i<jsonArray.length();i++){
                JSONObject object=jsonArray.getJSONObject(i);
                Map<String,Object> map=new HashMap<>();
                String result=object.getString("result");
                String times=object.getString("times");
                map.put("result",result);
                map.put("times",times);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }

}
