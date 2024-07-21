#include "interface.h"
#include "msg_handler.h"
#include "serialInterface.h"

hid_device *usb_handle = NULL;
hid_device_info *usb_info = NULL;

int rec_num = 0;    //接收数据长度
uint8_t rec_buffer[REC_LEN] = {0};
uint8_t hid_command[SEND_LEN] = {0};


Msg_Handler *msgHandler = NULL;
Form_MainWindow *mainwindow = NULL;
//SerialInterface *serialInterface = NULL;

Interface::Interface(QObject *parent) : QThread(parent)
{
    qRegisterMetaType<uint8_t>("uint8_t");

    stopped = false;
    mainwindow = new Form_MainWindow;
    mainwindow->show();
    msgHandler = new Msg_Handler;
    //connect(usbListener, &USB_Listener::deviceIn, this, &Interface::connectDevice);

    connect(msgHandler,&Msg_Handler::Signal_Update_Firmware_Msg,mainwindow,&Form_MainWindow::Slot_Update_FirmwareMsg);
    connect(msgHandler,&Msg_Handler::Signal_Update_Hardware_Msg,mainwindow,&Form_MainWindow::Slot_Update_HardwareMsg);

    connect(msgHandler, &Msg_Handler::SignalMsgText, mainwindow, &Form_MainWindow::SlotNotifyText);
}

void Interface::run()
{

    while(1)
    {
        if(usb_handle == NULL)  //未连接
        {
            //获取对应vid、pid下所有设备
            usb_info = hid_enumerate(PROTOCOL_VID,PROTOCOL_PID);

            if(usb_info == NULL)
            {
                qDebug() << "info get failed";
            }
            else
            {
                qDebug() << "info get success";
                //获取第三个设备(前两个分别是鼠标、键盘)
                usb_info = usb_info->next;
                usb_info = usb_info->next;

                qDebug("interface_number:%s",usb_info->path);//打印地址

                usb_handle = hid_open_path(usb_info->path);

                hid_free_enumeration(usb_info);
            }
            msleep(1000);
        }
        else    //已连接
        {
            //阻塞方式等待hid数据
            rec_num = hid_read_timeout(usb_handle,rec_buffer,REC_LEN,-1);

            if(rec_num > 0)
            {
                if(!msgHandler->checkHeadTail(rec_buffer))
                {

                    msgHandler->Data_Resolve(rec_buffer);
                }

            }
        }
    }
}

void Interface::connectDevice(QString vid, QString pid)
{
//    qDebug() << "try to connect device";
//    if (vid.toInt() == PROTOCOL_VID && pid.toInt() == PROTOCOL_PID)
//    {
//        // 获取系统中所有串口的信息
//        QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();

//        // 遍历所有串口，查找指定vid和pid的串口
//        for (const QSerialPortInfo &serialPortInfo : serialPortInfos)
//        {
//            if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
//            {
//                if (serialPortInfo.vendorIdentifier() == SERIAL_VID && serialPortInfo.productIdentifier() == SERIAL_PID)
//                {
//                    // 找到符合条件的串口，打开串口
//                    serial.setPortName(serialPortInfo.portName());
//                    if (serial.open(QSerialPort::ReadWrite)) {
//                        qDebug() << "Serial open success!";
//                        //设置波特率
//                        if (serial.setBaudRate(QSerialPort::Baud115200))
//                        {
//                            qDebug() << "Baudrate set success!";
//                            //设置数据位
//                            if (serial.setDataBits(QSerialPort::Data8))
//                            {
//                                qDebug() << "Databits set success!";
//                                //设置校验位
//                                if (serial.setParity(QSerialPort::NoParity))
//                                {
//                                    qDebug() << "Parity set success!";
//                                    //设置停止位
//                                    if (serial.setStopBits(QSerialPort::OneStop))
//                                    {
//                                        qDebug() << "Stopbits set success!";
//                                        //设置流控制
//                                        if (serial.setFlowControl(QSerialPort::NoFlowControl))
//                                        {
//                                            qDebug() << "Flow control set success!";
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                        break;
//                    }
//                    else
//                    {
//                        qDebug() << "Failed to open serial port.";
//                    }
//                }
//            }
//        }
//    }
}
