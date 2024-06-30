#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

#include <QObject>

extern  uint8_t TableState[8];

#define HID_CMD_HEAD 0xFE
#define FINGER_MAX_NUM  64

#define HID_PROTOCOL_START_H    (0xC0)
#define HID_PROTOCOL_START_L    (0x0C)
#define HID_PROTOCOL_STOP_H     (0xED)
#define HID_PROTOCOL_STOP_L     (0xDE)

#define PROTOCOL_REC_CMD_TEXT_START     (0x01)
#define PROTOCOL_REC_CMD_TEXT_FRAME     (0x02)

typedef enum USB_PROTOCOL_FORMAT
{
    USB_PROTOCOL_FORMAT_NOUSE0,				  //0x00
    USB_PROTOCOL_FORMAT_MODE_SWITCH,		  //0x01
    USB_PROTOCOL_FORMAT_GET_FW,               //0x02
    USB_PROTOCOL_FORMAT_GET_HW,               //0x03
    USB_PROTOCOL_FORMAT_NOUSE3,               //0x04
    USB_PROTOCOL_FORMAT_NOUSE4,               //0x05
    USB_PROTOCOL_FORMAT_NOUSE5,               //0x06
    USB_PROTOCOL_FORMAT_NOUSE6,               //0x07
    USB_PROTOCOL_FORMAT_NOUSE7,               //0x08
    USB_PROTOCOL_FORMAT_NOUSE8,               //0x09
    USB_PROTOCOL_FORMAT_NOUSE9,               //0x0A
    USB_PROTOCOL_FORMAT_ENROLL_FINGER,        //0x0B
    USB_PROTOCOL_FORMAT_DELETE_FINGER,    	  //0x0C
    USB_PROTOCOL_FORMAT_SET_FINGER_COLOR,     //0x0D
    USB_PROTOCOL_FORMAT_NOUSE11,			  //0x0E
    USB_PROTOCOL_FORMAT_SET_FINGER_COLOR_PRO, //0x0F
    USB_PROTOCOL_FORMAT_GET_INDEX_LIST,		  //0x10
    USB_PROTOCOL_FORMAT_FUNC_STORE,           //0x11
    USB_PROTOCOL_FORMAT_SET_RGB,              //0x12
    USB_PROTOCOL_FORMAT_SET_ACTION,           //0x13	设置Action按键功能

}USB_PROTOCOL_FORMAT_e;

typedef struct REC_DATA_FORMAT{
    uint8_t start[2];
    uint8_t cmd;
    uint8_t dataLen;
    uint8_t data[56];
    uint8_t checksum;	//checksum = start + cmd + data_len + data[...]
    uint8_t stop[2];
} REC_DATA_FORMAT_t;

typedef struct hidTransDataStruct{
    uint8_t reportId;
    uint8_t start[2];
    uint8_t cmd;
    uint8_t dataLen;
    uint8_t data[56];
    uint8_t checksum;	//checksum = start + cmd + data_len + data[...]
    uint8_t stop[2];
} HidTransDataStruct;

typedef struct recTextCtrlStruct
{
    uint16_t frameLen;  //数据帧总个数
    uint16_t curFrame;  //当前数据帧
}RecTextCtrlStruct;

class Msg_Handler : public QObject
{
    Q_OBJECT
public:
    explicit Msg_Handler(QObject *parent = nullptr);
    int checkHeadTail(uint8_t *data);
    int sendHidData(uint8_t cmd, uint8_t data[], uint8_t dataLen);
    int Comp_CheckSum(uint8_t *msg, uint8_t checksun);
    int Data_Resolve(uint8_t *data);


signals:
    void Signal_Update_TableState();
    void Signal_Update_EnrollState(uint8_t state1,uint8_t state2);
    void Signal_Update_Firmware_Msg(char* date,char* version);
    void Signal_Update_Hardware_Msg(uint8_t* flashId,char ch9329_ver,char* fpm383cSN);
    void SignalMsgText(uint8_t *data, uint8_t lenth);
};

#endif // MSG_HANDLER_H
