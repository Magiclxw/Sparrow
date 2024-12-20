#ifndef DRIVER_USB_H
#define DRIVER_USB_H

#include <QThread>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#define USB_VID  (0x303A)
#define USB_PID  (0x4005)

#define SERIAL_PROTOCOL_START_H     (0xC0)
#define SERIAL_PROTOCOL_START_L     (0x0C)
#define SERIAL_PROTOCOL_STOP_H      (0xED)
#define SERIAL_PROTOCOL_STOP_L      (0xDE)

#define SERIAL_CMD_TEXT_START       (0x01)
#define SERIAL_CMD_TEXT_FRAME       (0x02)
#define SERIAL_CMD_FILE_START       (0x03)
#define SERIAL_CMD_FILE             (0x04)
#define SERIAL_CMD_DISK_INFO        (0x07)
#define SERIAL_CMD_SYS_INFO         (0x08)
#define SERIAL_CMD_CLEAR_WIFI_INFO  (0x09)
#define USB_PROTOCOL_CMD_SET_WIFI_INFO (0x0A)
#define USB_PROTOCOL_CMD_SET_MQTT_INFO  (0x0B)
#define USB_PROTOCOL_CMD_SET_WEATHER_URL (0x0C)
#define USB_PROTOCOL_CMD_SET_BILIBILI_URL (0x0D)
#define USB_PROTOCOL_CMD_SET_SLEEP_TIME     (0x0E)
#define USB_PROTOCOL_CMD_SET_BEL_NAME       (0x0F)

typedef enum mqttDataEnum
{
    MQTT_DATA_ADDR,
    MQTT_DATA_USERNAME,
    MQTT_DATA_PASSWORD,
}MqttDataEnum;

typedef struct recTextCtrlStruct
{
    uint16_t frameLen;  //数据帧总个数
    uint16_t curFrame;  //当前数据帧
    int dataLen;   //数据总长度
}RecTextCtrlStruct;

typedef struct REC_DATA_FORMAT{
    uint8_t start[2];
    uint8_t cmd;
    uint8_t dataLen;
    uint8_t data[56];
    uint8_t checksum;	//checksum = start + cmd + data_len + data[...]
    uint8_t stop[2];
} REC_DATA_FORMAT_t;

extern QSerialPort serial;

class Driver_Usb : public QThread
{
public:
    explicit Driver_Usb(QObject *parent = nullptr);
    void usbConnectDevice();
    void sendDiskInfo();
    void sendSysInfo();
    void usbClearWifiInfo();
    void usbSetWifiInfo(uint8_t * ssid, uint8_t ssidLen, uint8_t* password, uint8_t passwordLen);
    void usbSetMqttInfo(MqttDataEnum dataType, uint8_t * data, uint8_t dataLen);
    void usbPcMonitorCtrl(uint8_t ctrl);
    void usbSetWeatherUrl(uint8_t* url, uint8_t urlLen);
    void usbSetBilibiliUrl(uint8_t* url, uint8_t urlLen);
    void usbSetBleName(uint8_t* name, uint8_t len);
    void usbSetSleepTime(uint32_t sleepTime);
    uint8_t usbGetTextRecFinishFlag();
    void usbSetTextRecFinishFlag(uint8_t flag);
    uint8_t * usbGetRecText();
    int usbGetTextRecDataLen();
    void run();
private:

    void serialReadData();
    void handleSerialError(QSerialPort::SerialPortError error);

};

#endif // DRIVER_USB_H
