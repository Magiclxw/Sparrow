package cn.itcast.mqttclient;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.net.URLEncoder;

public class HTTPConnect implements Runnable{
    //String path = MainActivity.httpweb;
    static InputStream inputStream;
    @Override
    public void run() {
//        try {
//            URL url = new URL(path);
//            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
//            conn.setConnectTimeout(5000);
//            conn.setRequestMethod("POST");
//            String data="param=" + URLEncoder.encode("test");
//            conn.setRequestProperty("Content-Type","application/x-www-form-urlencoded");
//            conn.setRequestProperty("Content-Length",data.length() + "");
//            conn.setDoOutput(true);
//            conn.setDoInput(true);
//            OutputStream os=conn.getOutputStream();
//            os.write(data.getBytes());
//            int code=conn.getResponseCode();
//            if(code==200){
//                InputStream is=conn.getInputStream();
//                inputStream=is;
//            }
//        } catch (ProtocolException protocolException) {
//            protocolException.printStackTrace();
//        } catch (MalformedURLException malformedURLException) {
//            malformedURLException.printStackTrace();
//        } catch (IOException ioException) {
//            ioException.printStackTrace();
//        }
    }
}
