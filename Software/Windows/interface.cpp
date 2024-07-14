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

    /* 更新索引表 */
    connect(msgHandler,&Msg_Handler::Signal_Update_TableState,mainwindow,&Form_MainWindow::Slot_UpdateIndexTable);
    /* 添加指纹 */
    connect(mainwindow,&Form_MainWindow::Signal_AddFinger,this,[=](uint8_t id,uint8_t times,uint8_t param1,uint8_t param2){
        HID_Add_Finger(usb_handle,id,param1,param2,times);
    });
    /* 删除指纹 */
    connect(mainwindow,&Form_MainWindow::Signal_DeleteFinger,this,[=](uint8_t id){
        HID_Delete_Finger(usb_handle,id);
    });
    /* 刷新指纹列表 */
    connect(mainwindow,&Form_MainWindow::Signal_RefreshFinger,this,[=](){
        HID_Get_TableState(usb_handle);
    });
    /* 设置设置呼吸灯效 */
    connect(mainwindow,&Form_MainWindow::Signal_SetBreathRGB,this,[=](uint8_t color_R,uint8_t color_G,uint8_t color_B,uint8_t interval){
        HID_Send_Breath_RGB(usb_handle,color_R,color_G,color_B,interval);
    });
    /* 设置开机密码 */
    connect(mainwindow,&Form_MainWindow::Signal_SetWindowsPassword,this,[=](QString password,uint8_t fingertype,uint8_t id){
        HID_Send_WindowsPassword(usb_handle,(Finger_Type_e)fingertype,password,id);
    });
    /* 设置密码 */
    connect(mainwindow,&Form_MainWindow::Signal_SetPassword,this,[=](QString password,uint8_t fingertype,uint8_t id){
        HID_Send_Password(usb_handle,(Finger_Type_e)fingertype,password,id);
    });
    /* 设置账号+密码 */
    connect(mainwindow,&Form_MainWindow::Signal_SetAccount_Password,this,[=](QString account,QString password,uint8_t fingertype,uint8_t id){
        HID_Send_Account_Password(usb_handle,(Finger_Type_e)fingertype,account,password,id);
    });
    /* 设置快捷启动 */
    connect(mainwindow,&Form_MainWindow::Signal_SetQuickStart,this,[=](uint8_t fingertype,QUICK_START_e startID,uint8_t index){
        HID_Send_QuickStart(usb_handle,(Finger_Type_e)fingertype,startID,index);
    });
    /* 更新指纹注册状态 */
    connect(msgHandler,&Msg_Handler::Signal_Update_EnrollState,mainwindow,&Form_MainWindow::Slot_EnrollState);
    /* 设置快捷键 */
    connect(mainwindow,&Form_MainWindow::Signal_SetShortcut,this,[=](uint8_t fingertype,uint8_t func,char* key,uint8_t key_len,uint8_t index){
        HID_Send_Shortcut(usb_handle,(Finger_Type_e)fingertype,func,key,key_len,index);
    });
    /* 设置Action按键功能 */
    connect(mainwindow,&Form_MainWindow::Signal_SetActionFunc,this,[=](uint8_t func,uint8_t action){
        HID_Set_Action_Func(usb_handle,func,action);
    });
    /* 设置指纹模块灯效 */
    connect(mainwindow,&Form_MainWindow::Signal_SetFingerRGB,this,[=](uint8_t mode,uint8_t startColor,uint8_t stopColor,uint8_t interval){
        HID_Send_Finger_RGB(usb_handle,mode,startColor,stopColor,interval);
    });
    /* 获取固件信息 */
    connect(mainwindow,&Form_MainWindow::Signal_GetFirmwareMsg,this,[=](){
        HID_Get_FW_Msg(usb_handle);
    });
    /* 获取硬件信息 */
    connect(mainwindow,&Form_MainWindow::Signal_GetHardwareMsg,this,[=](){
        HID_Get_HW_Msg(usb_handle);
    });

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
