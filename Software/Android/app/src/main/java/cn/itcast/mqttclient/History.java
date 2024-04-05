package cn.itcast.mqttclient;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.graphics.ImageDecoder;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

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

public class History extends AppCompatActivity {
    public ArrayList<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
    ListView lv;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.listview);
        init();
    }
    public boolean onKeyDown(int keyCode, KeyEvent event){
        if(keyCode==KeyEvent.KEYCODE_BACK){
            History.this.finish();
        }
        return false;
    }
    private void init() {
        list.clear();
        lv = (ListView) findViewById(R.id.lv);

        new Thread(new Runnable() {
            @Override
            public void run() {

//                OkHttpClient okHttpClient = new OkHttpClient();
////服务器返回的地址
//                Request request = new Request.Builder()
//                        .url(MainActivity.httpweb).build();
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

            }
        }).start();


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
                        String result=object.getString("result");
                        String way=object.getString("way");
//存入map
                        map.put("id", name);
                        map.put("times", times);
                        map.put("result",result);
                        map.put("way",way);
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

                convertView = getLayoutInflater().inflate(R.layout.item, null);
                viewHolder.id = (TextView) convertView.findViewById(R.id.id);
                viewHolder.result = (TextView) convertView.findViewById(R.id.result);
                viewHolder.times=(TextView)convertView.findViewById(R.id.times);
                viewHolder.way=(TextView)convertView.findViewById(R.id.way);
                convertView.setTag(viewHolder);
            } else {
                viewHolder = (ViewHolder) convertView.getTag();
            }

            viewHolder.id.setText(list.get(position).get("id").toString());
            viewHolder.times.setText(list.get(position).get("times").toString());
            viewHolder.result.setText(list.get(position).get("result").toString());
            viewHolder.way.setText(list.get(position).get("way").toString());
            if(list.get(position).get("result").toString().equals("成功")){
                viewHolder.result.setTextColor(0xFF4CAF50);
            }
            if(list.get(position).get("result").toString().equals("失败")){
                viewHolder.result.setTextColor(0xFFF44336);
            }
            return convertView;
        }

    }

    final class ViewHolder {
        TextView id;
        TextView result;
        TextView times;
        TextView way;
    }
    public void onBackPressed(){
        History.this.finish();
    }
}