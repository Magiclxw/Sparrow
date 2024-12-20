#include "driver_usb.h"
#include "hidapi.h"
#include <QDebug>
#include "system.h"
#include "interface/usblistener.h"
#include "interface/jqcpumonitor.h"
#include <QTimer>
#include "mainwindow.h"

QSerialPort serial;
QTimer *timer;

static uint8_t nameSize = 0;        //文件名长度
static QString fileName;            //文件名
static uint32_t dataSize = 0;       //数据长度
static uint16_t frameSize = 0;      //帧大小
static uint32_t totalFrame = 0;     //帧个数
static uint16_t currentIndex = 0;   //当前数据帧
static uint32_t recDataSize = 0;    //已接收数据大小
static uint16_t exteaData = 0;      //不足一帧数据大小
static uint8_t s_pcMonitor = 0;     //电脑监控开关
static uint8_t s_textStartFlag = 0;
static uint8_t s_textRecFinishFlag = 0;
static int s_textRecDataLen = 0;
uint8_t *s_filePointer;             //接收数据指针
static uint8_t textBuffer[2048];
RecTextCtrlStruct s_recTextCtrl;
REC_DATA_FORMAT_t g_rec_data_format;

QString failSavePath = "config/test.bin";

static bool compareCheckSum(uint8_t data[]);
static uint8_t generateChecksum(uint8_t data[], uint8_t len);
static void recDataHandler(uint8_t data[]);
static int sendCdcData(uint8_t cmd, uint8_t data[], uint16_t dataLen);
//static void sendDiskInfo();

Driver_Usb::Driver_Usb(QObject *parent) : QThread(parent)
{
    usbConnectDevice();
    JQCPUMonitor::initialize();

    timer = new QTimer();

    connect(&serial, &QSerialPort::readyRead, this, &Driver_Usb::serialReadData);
    connect(&serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),  this, &Driver_Usb::handleSerialError);
    connect(timer, &QTimer::timeout, this, &Driver_Usb::sendSysInfo);
//    timer->start(1000);
}


void Driver_Usb::run()
{

    while (1)
    {
//        usbClearWifiInfo();
//        qDebug() << "clear wifi";
//        sendSysInfo();
        //sysGetNetSpeed();
        //sysGetCpuInfo();
        //sendDiskInfo();
//        qDebug() << "\nCPU(Now):" << JQCPUMonitor::cpuUsagePercentage()*100;
//        qDebug() << "CPU(5S):" << JQCPUMonitor::cpuUsagePercentageIn5Second();
//        qDebug() << "CPU(30S):" << JQCPUMonitor::cpuUsagePercentageIn30Second();
        msleep(1000);
    }
}

//usb拔出错误处理
void Driver_Usb::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        serial.close();
        qDebug() << "error!";
    }
}

void Driver_Usb::serialReadData()
{
    QByteArray data = serial.readAll();
    qDebug() << "Received data: " << data;
    recDataHandler((uint8_t*)(data.data()));
}
/**
 * @brief Driver_Usb::usbConnectDevice
 *
 */
void Driver_Usb::usbConnectDevice()
{
    // 获取系统中所有串口的信息
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();
    // 遍历所有串口，查找指定vid和pid的串口
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            if (serialPortInfo.vendorIdentifier() == USB_VID && serialPortInfo.productIdentifier() == USB_PID) {
                // 找到符合条件的串口，打开串口
                serial.setPortName(serialPortInfo.portName());
                if (serial.open(QSerialPort::ReadWrite))
                {
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
                } else {
                    qDebug() << "Failed to open serial port.";
                }
            }
        }
    }
}

static bool compareCheckSum(uint8_t data[])
{
    uint8_t checkSum = 0;
    uint8_t recCheckSum = 0;
    uint16_t dataLen = data[3] << 8 | data[4];

    recCheckSum = data[dataLen + 5];

    qDebug() << "data length = " + QString::number(dataLen);
    for (int i = 0; i < dataLen + 5; i++)
    {
        checkSum += data[i];
    }

    if (recCheckSum == checkSum)
    {
        qDebug() << "compare pass";
        return true;
    }
    else
    {
        qDebug() << "compare fail";
        return false;
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

static void recDataHandler(uint8_t data[])
{
    uint8_t startH = data[0];
    uint8_t startL = data[1];
    uint8_t cmd = data[2];

    if (startH == SERIAL_PROTOCOL_START_H && startL == SERIAL_PROTOCOL_START_L)
    {
        //比较校验位
        if (compareCheckSum(data) == true)
        {
            switch (cmd)
            {
                case SERIAL_CMD_TEXT_START:
                {
                    uint8_t ack = 0;

                    s_recTextCtrl.frameLen = data[5] << 8 | data[6];
                    s_recTextCtrl.curFrame = 0;
                    s_recTextCtrl.dataLen = s_recTextCtrl.frameLen * 53;
                    //s_textPointer = (uint8_t*)malloc(s_recTextCtrl.dataLen);
                    qDebug() << "frame length : " << s_recTextCtrl.frameLen;
                    ack = 1;
                    sendCdcData(SERIAL_CMD_TEXT_START, &ack, 1);

                    s_textStartFlag = 1;
                }
                break;

                case SERIAL_CMD_TEXT_FRAME:
                {
                    if (s_textStartFlag == 1)
                    {
                        uint8_t response[3];
                        uint16_t dataLen = data[3] << 8 | data[4];
                        uint16_t frameIndex = data[5] << 8 | data[6];

                        qDebug() << "len:" << g_rec_data_format.dataLen;

                        if (frameIndex == s_recTextCtrl.curFrame)
                        {
                            memcpy(&textBuffer[s_recTextCtrl.curFrame * 53], &data[7], dataLen);   //*53 : 除去frameIndex字节

                            //收到最后一帧数据
                            if(s_recTextCtrl.curFrame == s_recTextCtrl.frameLen - 1)
                            {
                                QString data = QString::fromUtf8(reinterpret_cast<const char*>(textBuffer), s_recTextCtrl.dataLen);

                                qDebug() << "rec finished:" << data;
                                //1、发送信号(注意释放申请的空间)
//                                emit signalMsgText(textBuffer,s_recTextCtrl.dataLen);
                                s_textRecFinishFlag = 1;
                                s_textRecDataLen = s_recTextCtrl.dataLen;
                                //2、清空buffer
                                memset(&s_recTextCtrl, 0, sizeof(RecTextCtrlStruct));
                                s_textStartFlag = 0;
                            }
                        }

                        qDebug() << "current frame : " << s_recTextCtrl.curFrame;

                        qDebug() << "frame index : " << frameIndex;

                        QByteArray byteArray(reinterpret_cast<char*>(&data[7]), dataLen);

                        qDebug() << byteArray.toHex();

                        response[0] = s_recTextCtrl.curFrame >> 8;
                        response[1] = s_recTextCtrl.curFrame;
                        response[2] = 1;

                        sendCdcData(SERIAL_CMD_TEXT_FRAME, response, 3);

                        s_recTextCtrl.curFrame++;
                    }
                }
                break;

                case SERIAL_CMD_FILE_START:
                {
                    uint8_t ack = 1;

                    nameSize = data[5];

                    uint8_t name[nameSize];

                    for (uint8_t i = 0; i < nameSize; i++)
                    {
                        name[i] = data[6 + i];
                    }

                    fileName = QString::fromUtf8((char*)name, nameSize);

                    dataSize = data[6 + nameSize]<<24 | data[7 + nameSize]<<16 | data[8 + nameSize]<<8 | data[9 + nameSize];
                    frameSize = data[10 + nameSize]<<8 | data[11 + nameSize];
                    totalFrame = dataSize / frameSize;
                    exteaData = dataSize % frameSize;

                    s_filePointer = (uint8_t*)malloc((uint32_t)dataSize);

                    //监控开关打开，开始传输前关闭开关
                    if (s_pcMonitor == 1)
                    {
                        timer->stop();
                    }


                    sendCdcData(SERIAL_CMD_FILE_START, &ack, 1);

                    break;
                }
                case SERIAL_CMD_FILE:
                {
                    qDebug() << "received file";
                    uint8_t ack[3] = {0};

                    frameSize = data[3] << 8 | data[4];
                    currentIndex = data[5] << 8 | data[6];
                    recDataSize = currentIndex * 300;

                    qDebug() << "rec total size = " + QString::number(recDataSize);

                    if (s_filePointer != NULL)
                    {
                        memcpy(s_filePointer + recDataSize, &data[7], frameSize-2);
                    }

                    ack[0] = data[5];
                    ack[1] = data[6];
                    ack[2] = 1;
                    qDebug() << "currentIndex = " + QString::number(currentIndex);
                    qDebug() << "totalFrame = " + QString::number(totalFrame);
                    if (currentIndex != totalFrame)
                    {
                        sendCdcData(SERIAL_CMD_FILE, ack, 3);
                    }
                    else
                    {
                        sendCdcData(SERIAL_CMD_FILE, ack, 3);

                        if (s_pcMonitor == 1)
                        {
                            timer->start(1000);
                        }

                        sysSaveDataToFile(fileName, s_filePointer, dataSize);
                        if (s_filePointer != NULL)
                        {
                            free(s_filePointer);
                        }

                        currentIndex = 0;

                        qDebug() << "end";
                    }
                    break;
                }
                case SERIAL_CMD_DISK_INFO:
                {
                    QList<QStorageInfo> msg;
                    sysGetDiskMsg(&msg);
                    uint8_t diskNum = msg.count();
                    uint8_t *data = (uint8_t*)malloc(diskNum * 17 + 1); //1:diskNum
                    uint8_t diskPath;
                    uint64_t diskSize;
                    uint64_t remainSize;
                    data[0] = diskNum;
//                    for (uint8_t i = 0; i < diskNum; i++)
//                    {
//                        diskPath = msg.takeAt(i).rootPath().toUtf8().at(0);
//                        data[1 + i * 17] = diskPath;
//                        diskSize = msg.takeAt(i).bytesTotal() / (1024 * 1024);
//                        memcpy(&data[1 + i * 17 + 1], &diskSize, 8);
//                        remainSize = msg.takeAt(i).bytesAvailable() / (1024 * 1024);
//                        memcpy(&data[1 + i * 17 + 9], &remainSize, 8);
//                    }
                    for (uint8_t i = 0; i < diskNum; i++)
                    {
                        diskPath = msg.takeAt(i).rootPath().toUtf8().at(0);
                        diskSize = msg.takeAt(i).bytesTotal() / (1024 * 1024);
                        remainSize = msg.takeAt(i).bytesAvailable() / (1024 * 1024);
                        data[1 + i * 2] = diskPath;
                        data[1 + i * 2 + 1] = (uint8_t)(((float)remainSize/(float)diskSize)*100);

                    }
                    sendCdcData(SERIAL_CMD_DISK_INFO, data, 1 + diskNum * 2);

                    qDebug() << "send disk info";

                    free(data);
                    break;
                }

            }
        }
    }
}

int sendCdcData(uint8_t cmd, uint8_t data[], uint16_t dataLen)
{
    uint8_t sendData[1024];
    uint8_t sendSize = 0;

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
    sendData[dataLen+5+2] = SERIAL_PROTOCOL_STOP_L;

    sendSize = serial.write((char*)sendData, dataLen + 8);

    return sendSize;
}

static QList<QStorageInfo> msg;

void Driver_Usb::sendDiskInfo()
{
        sysGetDiskMsg(&msg);
        uint8_t diskNum = msg.count();
        uint8_t *data = (uint8_t*)malloc(diskNum * 2 + 1); //1:diskNum
        uint8_t diskPath;
        uint64_t diskSize;
        uint64_t remainSize;

        uint8_t memeryUsage = 0;
        uint8_t cpuUsage = 0;
        uint64_t uploadSpeed = 0;   //上传速度KB/s
        uint64_t downloadSpeed = 0; //下载速度KB/s

        data[0] = diskNum;
    //                    for (uint8_t i = 0; i < diskNum; i++)
    //                    {
    //                        diskPath = msg.takeAt(i).rootPath().toUtf8().at(0);
    //                        data[1 + i * 17] = diskPath;
    //                        diskSize = msg.takeAt(i).bytesTotal() / (1024 * 1024);
    //                        memcpy(&data[1 + i * 17 + 1], &diskSize, 8);
    //                        remainSize = msg.takeAt(i).bytesAvailable() / (1024 * 1024);
    //                        memcpy(&data[1 + i * 17 + 9], &remainSize, 8);
    //                    }
        for (uint8_t i = 0; i < diskNum; i++)
        {
            diskPath =msg.value(i).rootPath().toUtf8().at(0); //msg.takeAt(i).rootPath().toUtf8().at(0);

            diskSize =msg.value(i).bytesTotal() / (1024 * 1024);
            remainSize = msg.value(i).bytesAvailable() / (1024 * 1024);
            data[1 + i * 2] = diskPath;
            data[1 + i * 2 + 1] = 50;//(uint8_t)(100 - ((float)remainSize/(float)diskSize)*100);
        }
        sendCdcData(SERIAL_CMD_DISK_INFO, data, 1 + diskNum * 2);
        qDebug() << "send disk info";
        free(data);
}


void Driver_Usb::sendSysInfo()
{
    uint8_t data[18] = {0};
    uint64_t uploadSpeed, downLoadSpeed;
    uint8_t memery = 0;
    uint8_t cpu = 0;

    sysGetNetSpeed(&downLoadSpeed, &uploadSpeed);

    memery = sysGetMemeryUsage();

    cpu = JQCPUMonitor::cpuUsagePercentage()*100;

    qDebug() << "memery : " << QString::number(memery);
    qDebug() << "cpu : " << QString::number(cpu);

    data[0] = memery;
    data[1] = cpu;

    memcpy(&data[2], &downLoadSpeed, 8);
    memcpy(&data[10], &uploadSpeed, 8);

    sendCdcData(SERIAL_CMD_SYS_INFO, data, 18);
}

/**
 * @name Driver_Usb::usbClearWifiInfo
 * @brief 清除wifi账号密码
 */
void Driver_Usb::usbClearWifiInfo()
{
    uint8_t data = 1;
    sendCdcData(SERIAL_CMD_CLEAR_WIFI_INFO, &data, 1);
}

void Driver_Usb::usbSetWifiInfo(uint8_t * ssid, uint8_t ssidLen, uint8_t* password, uint8_t passwordLen)
{
    uint8_t data[ssidLen + passwordLen +2];

    data[0] = ssidLen;
    memcpy(&data[1], ssid, ssidLen);
    data[ssidLen] = '\0';
    data[ssidLen+1] = passwordLen;
    memcpy(&data[ssidLen+2], password, passwordLen);
    data[ssidLen + 1 + passwordLen] = '\0';

    sendCdcData(USB_PROTOCOL_CMD_SET_WIFI_INFO, data, ssidLen + passwordLen + 2);
}

void Driver_Usb::usbSetMqttInfo(MqttDataEnum dataType, uint8_t * data, uint8_t dataLen)
{
    uint8_t cmbData[dataLen + 2];

    cmbData[0] = dataType;
    cmbData[1] = dataLen;
    memcpy(&cmbData[2], data, dataLen);
    cmbData[dataLen + 1] = '\0';

    sendCdcData(USB_PROTOCOL_CMD_SET_MQTT_INFO, cmbData, dataLen + 2);
}

void Driver_Usb::usbSetWeatherUrl(uint8_t* url, uint8_t urlLen)
{
    uint8_t cmbData[urlLen + 1];

    cmbData[0] = urlLen;

    memcpy(&cmbData[1], url, urlLen);

    cmbData[urlLen + 1] = '\0';

    sendCdcData(USB_PROTOCOL_CMD_SET_WEATHER_URL, cmbData, urlLen + 1);
}

void Driver_Usb::usbSetBilibiliUrl(uint8_t* url, uint8_t urlLen)
{
    uint8_t cmbData[urlLen + 1];

    cmbData[0] = urlLen;

    memcpy(&cmbData[1], url, urlLen);

    cmbData[urlLen + 1] = '\0';

    sendCdcData(USB_PROTOCOL_CMD_SET_BILIBILI_URL, cmbData, urlLen + 1);
}

void Driver_Usb::usbSetSleepTime(uint32_t sleepTime)
{
    uint8_t cmbData[4];
    memcpy(cmbData, &sleepTime, 4);

    sendCdcData(USB_PROTOCOL_CMD_SET_SLEEP_TIME, cmbData, 4);
}

void Driver_Usb::usbSetBleName(uint8_t* name, uint8_t len)
{
    uint8_t cmbData[len + 1];

    cmbData[0] = len;

    memcpy(&cmbData[1], name, len);

    cmbData[len + 1] = '\0';

    sendCdcData(USB_PROTOCOL_CMD_SET_BEL_NAME, cmbData, len + 1);
}

void Driver_Usb::usbPcMonitorCtrl(uint8_t ctrl)
{
    if (ctrl == 0)
    {
        timer->stop();
    }
    else
    {
        timer->start(1000);
    }
    s_pcMonitor = ctrl;
}

uint8_t Driver_Usb::usbGetTextRecFinishFlag()
{
    return s_textRecFinishFlag;
}

int Driver_Usb::usbGetTextRecDataLen()
{
    return s_textRecDataLen;
}

void Driver_Usb::usbSetTextRecFinishFlag(uint8_t flag)
{
    s_textRecFinishFlag = flag;
}

uint8_t * Driver_Usb::usbGetRecText()
{
    return textBuffer;
}
