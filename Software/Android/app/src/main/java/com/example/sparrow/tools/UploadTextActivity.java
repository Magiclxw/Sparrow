package com.example.sparrow.tools;

import static com.example.sparrow.MainActivity.bleDataRecFlag;
import static com.example.sparrow.MainActivity.bleRecData;
import static com.example.sparrow.ble.BLEInterface.cmdHidSendText;
import static com.example.sparrow.ble.BLEInterface.cmdHidSendTextStart;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import androidx.appcompat.app.AppCompatActivity;

import com.example.sparrow.R;

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
                cmdHidSendTextStart(frameNum);
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
            if((short)(data[2] & 0xff) == 0x01 && data[4] == 0x01)
            {
                byte framData[] = new byte[FRAME_LEN];

                for (int i = 0; i < FRAME_LEN; i++)
                {
                    framData[i] = dataBuffer[frameIndex * FRAME_LEN + i];
                }

                cmdHidSendText(frameIndex, (byte) FRAME_LEN, framData);

                frameIndex++;
                System.out.println("frameIndex = " + frameIndex);
                return 0;
            }
            else if ((short)(data[2] & 0xff) == 0x02)
            {
                int recIndex = data[5]<<8 | data[6];
                System.out.println("recIndex = " + recIndex);
                System.out.println("frameIndex = " + frameIndex);
                if (frameIndex-1 == recIndex)
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
                            cmdHidSendText(frameIndex, (byte) FRAME_LEN, framData);
                            frameIndex++;
                        }
                        else if (frameIndex == frameNum - 1)
                        {
                            for (int i = 0; i < extraData; i++)
                            {
                                framData[i] = dataBuffer[frameIndex * FRAME_LEN + i];
                            }

                            cmdHidSendText(frameIndex, (byte)FRAME_LEN, framData);

                            System.out.println("send finished");

                            return 1;
                        }
                    }
                }
                else
                {
                    return 0;
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