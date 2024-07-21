package cn.itcast.mqttclient.toolsActivity;

import static java.security.AccessController.getContext;
import static cn.itcast.mqttclient.MainActivity.bleDataRecFlag;
import static cn.itcast.mqttclient.MainActivity.bleRecData;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_BIN;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_BMP;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_DOC;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_EXE;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_JPG;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_PDF;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_PNG;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_PPT;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_RAR;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_TXT;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_XLS;
import static cn.itcast.mqttclient.ble.BLEInterface.FILE_TYPE_ZIP;
import static cn.itcast.mqttclient.ble.BLEInterface.cmdCdcSendFile;
import static cn.itcast.mqttclient.ble.BLEInterface.cmdCdcSendFileStart;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Timer;
import java.util.TimerTask;

import cn.itcast.mqttclient.R;

public class UploadFileActivity extends AppCompatActivity {
    private static final int REQUEST_CODE = 1;
    //数据帧大小
    private static final int FRAME_SIZE = 300;
    private static long dataSize = 0;
    private static int fileType;
    private static String fileName;
    private static byte recData[];
    private static int recFlag = 0;
    Button btn_select_file;
    int frameIndex = 0;

    InputStream inputStream;
    BufferedInputStream bis;
    CountDownTimer timer;    //传输超时定时器
    BleTransportThread bleTransportThread = null;
    Uri uri;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_upload_file);

        recData = new byte[1024];
        btn_select_file = (Button) findViewById(R.id.upload_file_btn_select_file);

        btn_select_file.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("*/*");//筛选器
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                startActivityForResult(Intent.createChooser(intent, "选择一个文件"), 1);
            }
        });

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (timer != null) {
            timer.cancel();
        }

        Log.d("FileContent", "开始计数");

        timer = new CountDownTimer(2000, 1000) { // 30秒倒计时，每秒触发一次
            public void onTick(long millisUntilFinished) {
                // 每秒触发的操作
                recFlag = 0;
                Log.d("FileContent", "Counter up");
            }

            public void onFinish() {
                // 计时结束后触发的操作
//                if (recFlag == 0)
//                {
//                    frameIndex--;
//                    bleDataRecFlag = 1;
//                }
                Log.d("FileContent", "计时结束");
            }
        };

        timer.start();

        if (requestCode == REQUEST_CODE && resultCode == RESULT_OK) {
            uri = data.getData(); // 获取选定文件的 URI
            //获取文件大小
            dataSize = getFileSize(uri);
            fileName = uri.getPath().substring(uri.getPath().lastIndexOf("/") + 1);
            System.out.println("data size = " + dataSize);
            Log.d("FileContent", "类型: " + fileType);
            // 读取文件内容
            //readFileContent(uri);
            bleTransportThread = new BleTransportThread();
            bleTransportThread.start();
        }
    }

    private void readFileContent(Uri uri) {
        StringBuilder fileContent = new StringBuilder();
        try {
            inputStream = getContentResolver().openInputStream(uri);
            bis = new BufferedInputStream(inputStream);
            byte[] buffer = new byte[400];
            int bytesRead;
            int totalData = 0;
            while ((bytesRead = bis.read(buffer)) != -1) {
                totalData += bytesRead;
                Log.d("FileContent", "读取到的二进制数据: " + bytesRead + "字节");
            }
            Log.d("FileContent", "总大小: " + totalData + "字节");

            inputStream.close();
            bis.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
        //return fileContent.toString();
    }


    class BleTransportThread extends Thread {
        public void run() {
            try {
                inputStream = getContentResolver().openInputStream(uri);
                bis = new BufferedInputStream(inputStream);
                byte[] buffer = new byte[300];
                byte[] name = fileName.getBytes();
                int nameSize = name.length;
                int bytesRead = 0;
                int counter = 0;    //超时计数
                int timeOutFlag = 0;
                cmdCdcSendFileStart(nameSize, name, (int) dataSize, FRAME_SIZE);
                //cmdCdcSendFile(frameIndex, 100, buffer);
                //while ((bytesRead = bis.read(buffer)) != -1)
                {
                    //Log.d("FileContent", "bytesRead" + bytesRead);
                    while (true)
                    {
                        //if (timeOutFlag == 0)
                        {
                            counter++;
                            if (bleDataRecFlag == 1)
                            {

                                Log.d("FileContent", "收到响应");
                                if (bleRecData[2] == 0x03)
                                {
                                    bytesRead = bis.read(buffer);
                                    cmdCdcSendFile(frameIndex, bytesRead, buffer);
                                    bleDataRecFlag = 0;

                                    //timer.start();
                                }
                                else if (bleRecData[2] == 0x04)
                                {
                                    if ((bytesRead = bis.read(buffer)) != -1)
                                    {
                                        int ackFrameIndex =  ((bleRecData[5]&0xff) << 8) | (bleRecData[6]&0xff);
                                        Log.d("FileContent", "ackFrame = " + ackFrameIndex);
                                        if (frameIndex == ackFrameIndex)
                                        {
                                            frameIndex++;
                                            cmdCdcSendFile(frameIndex, bytesRead, buffer);
                                            bleDataRecFlag = 0;

                                            //timer.start();
                                            //Log.d("FileContent", "发送数据");
                                        }
                                        else
                                        {
                                            Log.d("FileContent", "索引不一致");
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                else
                                {
                                    Log.d("FileContent", "数据无效");
                                }
                                counter = 0;
                            }
                        }


                        if (counter == 0x7FFFFFF) {
                            Log.d("FileContent", "超时");
                            cmdCdcSendFile(frameIndex, bytesRead, buffer);
                            counter = 0;
                            //timeOutFlag= 1;
                        }
                    }
                }
                Log.d("FileContent", "结束");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


    public long getFileSize(Uri uri) {
        String[] projection = {MediaStore.Images.Media.SIZE};
        Cursor cursor = getContentResolver().query(uri, projection, null, null, null);

        long fileSize = 0;
        if (cursor != null && cursor.moveToFirst()) {
            int sizeIndex = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.SIZE);
            fileSize = cursor.getLong(sizeIndex);
            cursor.close();
        }
        return fileSize;
    }

}