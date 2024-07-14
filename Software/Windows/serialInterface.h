#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#define SERIAL_VID  (0x303A)
#define SERIAL_PID  (0x4005)

#define SERIAL_PROTOCOL_START_H    (0xD0)
#define SERIAL_PROTOCOL_START_L    (0x0D)
#define SERIAL_PROTOCOL_STOP_H     (0xED)
#define SERIAL_PROTOCOL_STOP_L     (0xDE)

#define SERIAL_CMD_FILE_START       (0x03)
#define SERIAL_CMD_FILE             (0x04)

extern QSerialPort serial;


typedef struct serialTransDataStruct{
    uint8_t start[2];
    uint8_t cmd;
    uint16_t dataLen;
    uint8_t *data;
    uint8_t checksum;	//checksum = start + cmd + data_len + data[...]
    uint8_t stop[2];
} SerialTransDataStruct;

class SerialInterface : public QObject
{
    Q_OBJECT
public:
    explicit SerialInterface(QObject *parent = nullptr);
    void handleSerialError(QSerialPort::SerialPortError error);
    void sendHidData(uint8_t cmd, uint8_t data[], uint8_t dataLen);
    int sendCdcData(uint8_t cmd, uint8_t data[], uint16_t dataLen);
private:
    void recDataHandler(uint8_t data[]);

private slots:
    void serialReadData();

signals:

};

#endif // SERIALINTERFACE_H
