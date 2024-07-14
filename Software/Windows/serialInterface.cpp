#include "serialInterface.h"
#include <QDebug>
#include <QObject>
#include "system/filemanager.h"

QSerialPort serial;
static uint8_t fileType = 0;
static uint32_t dataSize = 0;
static uint16_t frameSize = 0;
static uint32_t totalFrame = 0;
static uint16_t currentIndex = 0;
//已接收数据大小
static uint32_t recDataSize = 0;
//不足一帧数据大小
static uint16_t exteaData = 0;
//
uint8_t *s_filePointer;

QString path = "config/test.bin";

SerialInterface::SerialInterface(QObject *parent) : QObject(parent)
{
    connect(&serial, &QSerialPort::readyRead, this, &SerialInterface::serialReadData);
    connect(&serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),  this, &SerialInterface::handleSerialError);

    // 获取系统中所有串口的信息
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();
    // 遍历所有串口，查找指定vid和pid的串口
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            if (serialPortInfo.vendorIdentifier() == SERIAL_VID && serialPortInfo.productIdentifier() == SERIAL_PID) {
                // 找到符合条件的串口，打开串口
                serial.setPortName(serialPortInfo.portName());
                if (serial.open(QSerialPort::ReadWrite)) {
                    qDebug() << "Serial open success!";
                    //设置波特率
                    if (serial.setBaudRate(QSerialPort::Baud115200))
                    {
                        qDebug() << "Baudrate set success!";
                        //设置数据位
                        if (serial.setDataBits(QSerialPort::Data8))
                        {
                            qDebug() << "Databits set success!";
                            //设置校验位
                            if (serial.setParity(QSerialPort::NoParity))
                            {
                                qDebug() << "Parity set success!";
                                //设置停止位
                                if (serial.setStopBits(QSerialPort::OneStop))
                                {
                                    qDebug() << "Stopbits set success!";
                                    //设置流控制
                                    if (serial.setFlowControl(QSerialPort::NoFlowControl))
                                    {
                                        qDebug() << "Flow control set success!";

                                    }
                                }
                            }
                        }
                    }
                    break;
                } else {
                    qDebug() << "Failed to open serial port.";
                }
            }
        }
    }
}

void SerialInterface::serialReadData()
{
    QByteArray data = serial.readAll();
    qDebug() << "Received data: " << data;
    recDataHandler((uint8_t*)(data.data()));
}

//usb拔出错误处理
void SerialInterface::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        serial.close();
        qDebug() << "error!";
    }
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

bool compareCheckSum(uint8_t data[])
{
    uint8_t checkSum = 0;
    uint8_t recCheckSum = 0;
    uint8_t dataLen = data[3] << 8 | data[4];
    recCheckSum = data[dataLen + 5];
    qDebug() << "data length = " + dataLen;
    for (int i = 0; i < dataLen + 5; i++)
    {
        checkSum += data[i];
    }

    if (recCheckSum == recCheckSum)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int SerialInterface::sendCdcData(uint8_t cmd, uint8_t data[], uint16_t dataLen)
{
    uint8_t sendData[1024];

    sendData[0] = SERIAL_PROTOCOL_START_H;
    sendData[1] = SERIAL_PROTOCOL_START_L;
    sendData[2] = cmd;
    sendData[3] = dataLen >> 8;
    sendData[4] = (uint8_t)dataLen;

    for (uint16_t i = 0; i < dataLen; i++)
    {
        sendData[i+5] = data[i];
    }

    sendData[dataLen+5] = generateChecksum((uint8_t*)&sendData, dataLen + 5);

    sendData[dataLen+5+1] = SERIAL_PROTOCOL_STOP_H;
    sendData[dataLen+5+1] = SERIAL_PROTOCOL_START_L;

    serial.write((char*)sendData, dataLen + 8);

    return 1;
}

void SerialInterface::recDataHandler(uint8_t data[])
{
    uint8_t startH = data[0];
    uint8_t startL = data[1];
    uint8_t cmd = data[2];

    if (startH == SERIAL_PROTOCOL_START_H && startL == SERIAL_PROTOCOL_START_L)
    {
        //比较校验位
        if (compareCheckSum(data))
        {
            switch (cmd)
            {
                case SERIAL_CMD_FILE_START:
                {
                    uint8_t ack = 1;

                    fileType = data[5];
                    dataSize = data[6]<<24 | data[7]<<16 | data[8]<<8 | data[9];
                    frameSize = data[10]<<8 | data[11];
                    totalFrame = dataSize / frameSize;
                    exteaData = dataSize % frameSize;

                    s_filePointer = (uint8_t*)malloc((uint32_t)dataSize);

                    sendCdcData(SERIAL_CMD_FILE_START, &ack, 1);

                    break;
                }
                case SERIAL_CMD_FILE:
                {
                    qDebug() << "received file";
                    uint8_t ack[3] = {0};

                    frameSize = data[3] << 8 | data[4];
                    currentIndex = data[5] << 8 | data[6];
                    recDataSize = currentIndex * (frameSize-2);

                    qDebug() << "rec total size = " + QString::number(recDataSize);

                    memcpy(s_filePointer + recDataSize, &data[7], frameSize-2);

                    ack[0] = data[5];
                    ack[1] = data[6];
                    ack[2] = 1;
                    qDebug() << "currentIndex = " + QString::number(currentIndex);
                    qDebug() << "totalFrame = " + QString::number(totalFrame);
                    if (currentIndex != totalFrame-1)
                    {
                        sendCdcData(SERIAL_CMD_FILE, ack, 3);
                    }
                    else
                    {
                        saveDataToFile(path, s_filePointer, dataSize);
                        qDebug() << "end";
                    }
                    break;
                }
            }
        }
    }
}

