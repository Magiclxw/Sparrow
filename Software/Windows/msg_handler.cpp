#include "msg_handler.h"
#include "interface.h"
#include "config/sys_config.h"
#include <QDebug>

REC_DATA_FORMAT_t g_rec_data_format;
HidTransDataStruct hidTransData;

RecTextCtrlStruct s_recTextCtrl;
uint8_t TableState[8] = {0};

static uint8_t *s_textPointer;

static uint8_t generateChecksum(uint8_t data[], uint8_t len);

Msg_Handler::Msg_Handler(QObject *parent) : QObject(parent)
{

}

int Msg_Handler::checkHeadTail(uint8_t *data)
{
    if (data[1] == HID_PROTOCOL_START_H &&
        data[2] == HID_PROTOCOL_START_L &&
        data[61] == HID_PROTOCOL_STOP_H &&
        data[62] == HID_PROTOCOL_STOP_L)
    {
        return OPERATE_SUCCESS;
    }
    else
    {
        return OPERATE_ERROR_INVALID_PARAMETERS;
    }
}

int Msg_Handler::sendHidData(uint8_t cmd, uint8_t data[], uint8_t dataLen)
{
    hidTransData.reportId = 0x03;
    hidTransData.start[0] = HID_PROTOCOL_START_H;
    hidTransData.start[1] = HID_PROTOCOL_START_L;
    hidTransData.cmd = cmd;
    hidTransData.dataLen = dataLen;
    for(uint8_t i = 0; i < dataLen; i++)
    {
        hidTransData.data[i] = data[i];
    }

    hidTransData.checksum = generateChecksum((uint8_t*)&hidTransData, dataLen + 4);

    hidTransData.stop[0] = HID_PROTOCOL_STOP_H;
    hidTransData.stop[1] = HID_PROTOCOL_STOP_L;

    if (usb_handle != NULL)
    {
        hid_write(usb_handle, (uint8_t*)&hidTransData, 64);

        return 1;
    }

    return 0;
}

static uint8_t generateChecksum(uint8_t data[], uint8_t len)
{
    uint8_t checkSum = 0;

    for(uint8_t i = 0; i < len; i++)
    {
        checkSum += data[i];
    }

    return checkSum;
}

/**
*@brief	比较校验和
*@param	-msg：待校验数据
*@return 执行状态
*/
int Msg_Handler::Comp_CheckSum(uint8_t *msg, uint8_t checksun)
{
    uint8_t length = msg[3]+4;    //获取接收长度
    uint8_t cmpCheckSum = 0;
    for(int i = 0;i < length; i++){
        cmpCheckSum += msg[i];
    }
    if(cmpCheckSum == checksun){
        qDebug() << "compare ok"  << "calc checksum = " << cmpCheckSum << "rec checksum = " << checksun;
        return OPERATE_SUCCESS;
    }else{
        qDebug() << "compare error" << "calc checksum = " << cmpCheckSum << "rec checksum = " << checksun;
        return OPERATE_ERROR_INVALID_PARAMETERS;
    }

}

/**
*@brief	HID数据处理
*@param	-data:接收数据指针
*@return 执行状态
*/
int Msg_Handler::Data_Resolve(uint8_t *data)
{
    uint8_t reportId = data[0];   //hid数据中第一个字节代表report id
    g_rec_data_format.start[0] = data[1];
    g_rec_data_format.start[1] = data[2];
    g_rec_data_format.cmd = data[3];
    g_rec_data_format.dataLen = data[4];

    memcpy((uint8_t*)&g_rec_data_format.data, (uint8_t*)&data[5],g_rec_data_format.dataLen);

    g_rec_data_format.checksum = data[60];

    if (Comp_CheckSum((uint8_t*)&g_rec_data_format, g_rec_data_format.checksum))
    {
        return OPERATE_ERROR_INVALID_PARAMETERS;
    }

    switch (g_rec_data_format.cmd)
    {
        case PROTOCOL_REC_CMD_TEXT_START:
        {
            uint8_t ack = 0;

            s_recTextCtrl.frameLen = data[5] << 8 | data[6];
            s_recTextCtrl.curFrame = 0;
            s_textPointer = (uint8_t*)malloc(s_recTextCtrl.frameLen * 53);
            qDebug() << "frame length : " << s_recTextCtrl.frameLen;
            ack = 1;
            sendHidData(PROTOCOL_REC_CMD_TEXT_START, &ack, 1);
        }
        break;

        case PROTOCOL_REC_CMD_TEXT_FRAME:
        {
            uint8_t ack = 0;
            uint16_t frameIndex = data[5] << 8 | data[6];

            if (frameIndex == s_recTextCtrl.curFrame)
            {
                memcpy(&s_textPointer[s_recTextCtrl.curFrame * 53], &data[7], g_rec_data_format.dataLen);   //*53 : 除去frameIndex字节

                s_recTextCtrl.curFrame++;
            }



            qDebug() << "current frame : " << s_recTextCtrl.curFrame;

            qDebug() << "frame index : " << frameIndex;

            QByteArray byteArray(reinterpret_cast<char*>(&data[7]), g_rec_data_format.dataLen);

            qDebug() << byteArray.toHex();

            //收到最后一帧数据
            if(s_recTextCtrl.curFrame == s_recTextCtrl.frameLen)
            {
                //1、发送信号(注意释放申请的空间)

                //2、清空buffer
                memset(&s_recTextCtrl, 0, sizeof(RecTextCtrlStruct));
            }

            sendHidData(PROTOCOL_REC_CMD_TEXT_FRAME, &ack, 1);
        }
        break;

    }
    memset(rec_buffer,0,REC_LEN);
    return OPERATE_SUCCESS;
}



