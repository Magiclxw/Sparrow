package cn.itcast.mqttclient.toolsActivity;

import static cn.itcast.mqttclient.MainActivity.bleDataRecFlag;
import static cn.itcast.mqttclient.MainActivity.bleRecData;
import static cn.itcast.mqttclient.ble.BLEInterface.cmdHidSendText;
import static cn.itcast.mqttclient.ble.BLEInterface.cmdHidSendTextStart;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.os.Bundle;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import cn.itcast.mqttclient.R;
import cn.itcast.mqttclient.ble.OnBleConnectListener;

public class UploadTextActivity extends AppCompatActivity {
    Button btn_upload;
    EditText et_upload;

    private int FRAME_LEN = 53;
    private int DATA_LEN = 55;

    String text;
    byte[] dataBuffer;
    int frameNum = 0;
    int extraData = 0;
    int frameIndex = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_upload_text);

        btn_upload = (Button) findViewById(R.id.upload_btn_upload);
        et_upload = (EditText) findViewById(R.id.upload_et_input);
        ClipboardManager clipboardManager = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);

        BleTransportThread bleTransportThread = new BleTransportThread();

        if(clipboardManager.hasPrimaryClip())
        {
            ClipData clipData = clipboardManager.getPrimaryClip();
            ClipData.Item item = clipData.getItemAt(0);
            if(item.getText() != null)
            {
                text = item.getText().toString();
                et_upload.setText(text);
            }
        }

        btn_upload.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                dataBuffer = new byte[text.length()];

                dataBuffer = text.getBytes();

                frameNum = dataBuffer.length/FRAME_LEN;

                extraData = dataBuffer.length % FRAME_LEN;
                if (extraData > 0)
                {
                    frameNum += 1;
                }
            cmdHidSendTextStart(frameNum, (byte) 2);
                //cmdHidSendText(frameIndex, (byte) FRAME_LEN, dataBuffer);
                if (!bleTransportThread.isAlive())
                {
                    bleTransportThread.start();
                }
            }
    });
    }

    int sendFrameData(byte[] data)
    {
        if ((short)(data[0] & 0xff) == 0xA5 && (short)(data[1] & 0xff) == 0x5A)
        {
            if (frameIndex <= frameNum - 1)
            {
                byte framData[] = new byte[FRAME_LEN];

                if (frameIndex < frameNum - 1)
                {
                    for (int i = 0; i < FRAME_LEN; i++)
                    {
                        framData[i] = dataBuffer[frameIndex * FRAME_LEN + i];
                    }
                    cmdHidSendText(frameIndex, (byte) DATA_LEN, framData);
                    frameIndex++;
                }
                else if (frameIndex == frameNum - 1)
                {
                    for (int i = 0; i < extraData; i++)
                    {
                        framData[i] = dataBuffer[frameIndex * FRAME_LEN + i];
                    }

                    cmdHidSendText(frameIndex, (byte)DATA_LEN, framData);

                    System.out.println("send finished");

                    return 1;
                }
            }
        }
        return 0;
    }

    class BleTransportThread extends Thread
    {
        public void run()
        {
            //cmdHidSendTextStart(frameLen);
            while (true)
            {
                if(bleDataRecFlag == 1)
                {
                    bleDataRecFlag = 0;
                    System.out.println("upload rec data");

                    if (sendFrameData(bleRecData) == 1)
                    {
                        frameIndex = 0;
                        frameNum = 0;
                        extraData = 0;

                        interrupt();
                    }
                }
                //cmdHidSendText();
            }
        }
    }

}