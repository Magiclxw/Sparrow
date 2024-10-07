package com.project.sparrow.system;

import android.os.Bundle;
import android.os.Message;

import com.project.sparrow.MainActivity;


public class UpdateState implements Runnable{
    static String rec_message = "null";
    static String rec_topic = "null";
    static String tmp_message = "null";
    @Override
    public void run() {
        while(!Thread.currentThread().isInterrupted()){
            Message message=new Message();
            Bundle bundle=new Bundle();

            rec_topic = OnMessageCallback.topic;
            rec_message = OnMessageCallback.message;

            if(rec_message != tmp_message) {
                bundle.putString(rec_topic,rec_message);
                message.setData(bundle);
                tmp_message = rec_message;
                MainActivity.handler.sendMessage(message);
                System.out.println("线程收到" + rec_message);
            }

//            try {
//                Thread.sleep(1000);
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
        }
    }
}
