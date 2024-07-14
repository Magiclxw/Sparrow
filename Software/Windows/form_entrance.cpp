#include "form_entrance.h"
#include "ui_form_entrance.h"
#include "lib/hidapi.h"
#include <lib/GlobalMouseKey/globalmouseevent.h>
#include "lib/GlobalMouseKey/globalkeyevent.h"
#include "interface.h"
#include "serialInterface.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

USB_Listener *usbListener;
SerialInterface *serialInterface;
QTimer timer;

Form_Entrance::Form_Entrance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Entrance)
{
    ui->setupUi(this);
    this->hide();
    Interface *usbInterface = new Interface;
    usbInterface->start();
    serialInterface = new SerialInterface;

    usbListener = new USB_Listener(this);
    usbListener->registerDevice(this->winId());
    qApp->installNativeEventFilter(usbListener);

    connect(usbListener,SIGNAL(deviceIn(QString,QString)),this,SLOT(onDeviceIn(QString,QString)));
    connect(usbListener,SIGNAL(deviceOut(QString,QString)),this,SLOT(onDeviceOut(QString,QString)));
}

void Form_Entrance::onDeviceIn(QString VID, QString PID)
{
    qDebug() << "device in VID :" + VID + "PID : " + PID;

    //if ((VID == "VID_303A") && (VID == "PID_4005"))

    //需要增加延时，否则信息不能及时获取
    for (int i = 0; i < 1000; i++)
    {
        qDebug() << "try to connect device";
    }

    // 获取系统中所有串口的信息
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();

    // 遍历所有串口，查找指定vid和pid的串口
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos)
    {
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if (serialPortInfo.vendorIdentifier() == SERIAL_VID && serialPortInfo.productIdentifier() == SERIAL_PID)
            {
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
                }
                else
                {
                    qDebug() << "Failed to open serial port.";
                }
            }
        }
    }
}


void Form_Entrance::onDeviceOut(QString VID, QString PID)
{
    qDebug() << "device out VID :" + VID + "PID : " + PID;

    //serial.close();

}
Form_Entrance::~Form_Entrance()
{
    delete ui;
}
