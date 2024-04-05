package cn.itcast.mqttclient;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class ApplyJson extends AppCompatActivity {
    public ArrayList<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
    ListView lv;
    MqttClient client = null;
    String opencontent="open";
    String pubTopic="LockControl";
    int qos = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.applylistview);

        String broker = MainActivity.mqttweb;
        String clientId = "Controller";
        MemoryPersistence persistence = new MemoryPersistence();
        try {
            client = new MqttClient(broker, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            // 设置回调
            client.setCallback(new OnMessageCallback());
            // 建立连接
            System.out.println("Connecting to broker: " + broker);
            try {
                client.connect(connOpts);
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
        init();
    }
    public boolean onKeyDown(int keyCode, KeyEvent event){
        if(keyCode==KeyEvent.KEYCODE_BACK){
            ApplyJson.this.finish();
        }
        return false;
    }
    private void init() {
//        list.clear();
//        lv = (ListView) findViewById(R.id.applylv);
//
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//
//                OkHttpClient okHttpClient = new OkHttpClient();
////服务器返回的地址
//                Request request = new Request.Builder()
//                        .url(MainActivity.httpweb).build();//web地址
//                try {
//                    Response response = okHttpClient.newCall(request).execute();
////获取到数据
//                    String date = response.body().string();
////把数据传入解析josn数据方法
//                    jsonJX(date);
//
//                } catch (IOException e) {
//// TODO Auto-generated catch block
//                    e.printStackTrace();
//                }
//
//            }
//        }).start();
//

    }

    private void jsonJX(String date) {
//判断数据是空
        if (date != null) {
            try {
                JSONArray array = new JSONArray(date);
//遍历
                for (int i = 0; i < array.length(); i++) {
                    JSONObject object = array.getJSONObject(i);

                    Map<String, Object> map = new HashMap<String, Object>();

                    try {
//获取到json数据中的activity数组里的内容name
                        String name = object.getString("id");
//获取到json数据中的activity数组里的内容startTime
                        String times = object.getString("times");
                        String reason=object.getString("reason");
                        String username=object.getString("username");
//存入map
                        map.put("id", name);
                        map.put("times", times);
                        map.put("reason",reason);
                        map.put("username",username);
//ArrayList集合
                        list.add(map);

                    } catch (JSONException e) {
// TODO Auto-generated catch block
                        e.printStackTrace();
                    }

                }

                Message message = new Message();
                message.what = 1;
                handler.sendMessage(message);

            } catch (JSONException e) {
                e.printStackTrace();
            }


        }


    }

    //Handler运行在主线程中(UI线程中)，  它与子线程可以通过Message对象来传递数据
    @SuppressLint("HandlerLeak")
    public Handler handler = new Handler() {


        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 1:
                    Mybaseadapter list_item = new Mybaseadapter();
                    lv.setAdapter(list_item);
                    break;
            }


        }
    };

    //listview的适配器
    class Mybaseadapter extends BaseAdapter {

        @Override
        public int getCount() {
            return list.size();

        }

        @Override
        public Object getItem(int position) {
            return list.get(position);
        }

        @Override
        public long getItemId(int position) {

            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            ViewHolder viewHolder = null;
            if (convertView == null) {
                viewHolder = new ViewHolder();

                convertView = getLayoutInflater().inflate(R.layout.applyitem, null);
                viewHolder.id = (TextView) convertView.findViewById(R.id.applyid);
                viewHolder.reason = (TextView) convertView.findViewById(R.id.reason);
                viewHolder.times=(TextView)convertView.findViewById(R.id.applytimes);
                viewHolder.username=(TextView)convertView.findViewById(R.id.username);
                convertView.setTag(viewHolder);
            } else {
                viewHolder = (ViewHolder) convertView.getTag();
            }

            convertView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    AlertDialog dialog;
                    AlertDialog.Builder builder=new AlertDialog.Builder(ApplyJson.this)
                            .setTitle("是否开锁?")
                            .setPositiveButton("是", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    MqttMessage message = new MqttMessage(opencontent.getBytes());
                                    message.setQos(qos);
                                    try {
                                        client.publish(pubTopic, message);
                                        System.out.println("Message published");
                                    } catch (MqttException e) {
                                        e.printStackTrace();
                                    }
                                    dialog.dismiss();
                                }
                            })
                            .setNegativeButton("否", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    dialog.dismiss();
                                }
                            });
                    dialog=builder.create();
                    dialog.show();
                }
            });

            viewHolder.id.setText(list.get(position).get("id").toString());
            viewHolder.times.setText(list.get(position).get("times").toString());
            viewHolder.reason.setText(list.get(position).get("reason").toString());
            viewHolder.username.setText(list.get(position).get("username").toString());
            return convertView;
        }

    }

    final class ViewHolder {
        TextView id;
        TextView reason;
        TextView times;
        TextView username;
    }
    public void onBackPressed(){
        ApplyJson.this.finish();
    }
}