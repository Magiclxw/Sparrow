package cn.itcast.mqttclient.ble;

import cn.itcast.mqttclient.MainActivity;

public class BLEInterface {
    private static final short CMD_START_H = 0xA5;
    private static final short CMD_START_L = 0x5A;
    private static final short CMD_STOP_H = 0xF0;
    private static final short CMD_STOP_L = 0x0F;

    private static final char CMD_MOUSE_MOVE = 0x01;
    private static final char CMD_MOUSE_CLICK = 0x02;
    private static final char CMD_KEYBOARD_INPUT = 0x03;    //按键输入
    private static final char CMD_KEYBOARD_FUNC = 0x04;     //功能键输入
    private static final char CMD_HID_DATA_SEND = 0x05;
    private static final char CMD_CFG_SET_SERVER = 0x06;
    private static final char CMD_CFG_SET_USERNAME = 0x07;
    private static final char CMD_CFG_SET_PASSWORD = 0x08;
    private static final char CMD_HID_SEND_TEXT_START = 0x09;
    private static final char CMD_HID_SEND_TEXT = 0x0A;
    private static final char CMD_CDC_SEND_FILE_START = 0x0B;
    private static final char CMD_CDC_SEND_FILE = 0x0C;

    public static int MOUSE_LEFT_BUTTON = 1;
    public static int MOUSE_RIGHT_BUTTON = 2;
    public static int MOUSE_MID_BUTTON = 3;

    public static int MOUSE_KEY_PRESSED = 0;
    public static int MOUSE_KEY_RELEASED = 1;
    public static int MOUSE_KEY_CLICKED = 2;

    public static int FILE_TYPE_TXT = 1;
    public static int FILE_TYPE_DOC = 2;
    public static int FILE_TYPE_PDF = 3;
    public static int FILE_TYPE_PPT = 4;
    public static int FILE_TYPE_XLS = 5;
    public static int FILE_TYPE_JPG = 10;
    public static int FILE_TYPE_PNG = 11;
    public static int FILE_TYPE_BMP = 12;
    public static int FILE_TYPE_ZIP = 20;
    public static int FILE_TYPE_RAR = 21;
    public static int FILE_TYPE_EXE = 30;
    public static int FILE_TYPE_BIN = 31;

    //功能按键
    public static byte HID_KEY_BACKSPACE = 0x2A;
    /**
     * 计算校验和
     * @param data  校验数据
     * @param length    数据长度
     */
    private static char CalcCheckSum(byte data[], int length)
    {
        char checkSum = 0;

        for(int i = 0; i < length; i++)
        {
            checkSum += data[i];
        }
        return checkSum;
    }
    /**
     * 鼠标移动
     * @param x  x方向移动距离
     * @param y  y方向移动距离
     */
    public static void cmdMouseMove(byte x, byte y)
    {
        byte data[] = new byte[8];
        data[0] = (byte)CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_MOUSE_MOVE;
        data[3] =  x;
        data[4] =  y;
        data[5] = (byte)CalcCheckSum(data,5);
        data[6] = (byte)CMD_STOP_H;
        data[7] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }
    /**
     * 鼠标移动
     * @param key  具体按键
     * @param state  按下/释放状态
     */
    public static void cmdMouseClick(int key, int state)
    {
        byte data[] = new byte[8];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_MOUSE_CLICK;
        data[3] = (byte)key;
        data[4] = (byte)state;
        data[5] = (byte) CalcCheckSum(data,5);
        data[6] = (byte)CMD_STOP_H;
        data[7] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

//    public static void cmdKeyboardInput(byte key)
//    {
//        byte data[] = new byte[7];
//        data[0] = (byte) CMD_START_H;
//        data[1] = (byte)CMD_START_L;
//        data[2] = CMD_KEYBOARD_INPUT;
//        data[3] = key;
//        data[4] = (byte) CalcCheckSum(data,4);
//        data[5] = (byte)CMD_STOP_H;
//        data[6] = (byte)CMD_STOP_L;
//        MainActivity.bleManager.sendCmd(data);
//
//    }

    public static void cmdKeyboardFunc(byte modifier,byte key)
    {
        byte data[] = new byte[8];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_KEYBOARD_FUNC;
        data[3] = modifier;
        data[4] = key;
        data[5] = (byte) CalcCheckSum(data,5);
        data[6] = (byte)CMD_STOP_H;
        data[7] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

    public static void cmdHidDataSend(short message[],byte length)
    {
        byte data[] = new byte[7+length];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_HID_DATA_SEND;
        data[3] = length;
        for(byte i = 0; i < length; i++)
        {
            data[i+4] =(byte) message[i];
        }
        data[4+length] = (byte) CalcCheckSum(data,4+length);
        data[4+length+1] = (byte)CMD_STOP_H;
        data[4+length+2] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

    public static void cmdHidSendTextStart(int frameLength, byte cmdLen)
    {
        byte data[] = new byte[9];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_HID_SEND_TEXT_START;
        data[3] = cmdLen;
        data[4] = (byte) (frameLength>>8);
        data[5] = (byte) frameLength;
        data[6] = (byte) CalcCheckSum(data,6);
        data[7] = (byte)CMD_STOP_H;
        data[8] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

    public static void cmdHidSendText(int frameIndex, byte dataLen, byte frameData[])
    {
        byte data[] = new byte[dataLen + 7];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_HID_SEND_TEXT;

        data[3] = (byte) dataLen;
        /***************** data ***************/
        data[4] = (byte) (frameIndex>>8);
        data[5] = (byte) frameIndex;
        for(int i = 0; i < dataLen-2; i++)
        {
            data[6+i] = frameData[i];
        }
        /***************************************/
        data[4 + dataLen] = (byte) CalcCheckSum(data,dataLen + 4);
        data[4 + dataLen + 1] = (byte)CMD_STOP_H;
        data[4 + dataLen + 2] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

    public static void cmdCdcSendFileStart(int fileType, int dataSize, int frameSize)
    {
        byte data[] = new byte[15];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_CDC_SEND_FILE_START;
        data[3] = 0x00;
        data[4] = 0x07;
        data[5] = (byte) fileType;
        data[6] = (byte) (dataSize>>24);
        data[7] = (byte) (dataSize>>16);
        data[8] = (byte) (dataSize>>8);
        data[9] = (byte) dataSize;
        data[10] = (byte) (frameSize>>8);
        data[11] = (byte) frameSize;
        data[12] = (byte) CalcCheckSum(data,12);
        data[13] = (byte)CMD_STOP_H;
        data[14] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

    public static void cmdCdcSendFile(int frameIndex, int frameLen, byte[] frameData)
    {
        byte data[] = new byte[frameLen + 10];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_CDC_SEND_FILE;

        data[3] = (byte) ((frameLen + 2) >> 8);
        data[4] = (byte) (frameLen + 2);

        /***************** data ***************/
        data[5] = (byte) (frameIndex>>8);
        data[6] = (byte) frameIndex;

        if (frameLen >= 0) System.arraycopy(frameData, 0, data, 7, frameLen);
        /***************************************/

        data[7 + frameLen] = (byte) CalcCheckSum(data,frameLen + 7);
        data[7 + frameLen + 1] = (byte)CMD_STOP_H;
        data[7 + frameLen + 2] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

    public static void cmdSetServerAddr(byte addr[], byte length)
    {
        byte data[] = new byte[7+length];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_CFG_SET_SERVER;
        data[3] = length;
        for(byte i = 0; i < length; i++)
        {
            data[i+4] = addr[i];
        }
        data[4+length] = (byte) CalcCheckSum(data,4+length);
        data[4+length+1] = (byte)CMD_STOP_H;
        data[4+length+2] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

    public static void cmdSetServerUsername(byte username[], byte length)
    {
        byte data[] = new byte[7+length];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_CFG_SET_USERNAME;
        data[3] = length;
        for(byte i = 0; i < length; i++)
        {
            data[i+4] = username[i];
        }
        data[4+length] = (byte) CalcCheckSum(data,4+length);
        data[4+length+1] = (byte)CMD_STOP_H;
        data[4+length+2] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

    public static void cmdSetServerPassword(byte password[], byte length)
    {
        byte data[] = new byte[7+length];
        data[0] = (byte) CMD_START_H;
        data[1] = (byte)CMD_START_L;
        data[2] = CMD_CFG_SET_PASSWORD;
        data[3] = length;
        for(byte i = 0; i < length; i++)
        {
            data[i+4] = password[i];
        }
        data[4+length] = (byte) CalcCheckSum(data,4+length);
        data[4+length+1] = (byte)CMD_STOP_H;
        data[4+length+2] = (byte)CMD_STOP_L;
        MainActivity.bleManager.sendCmd(data);
    }

}
