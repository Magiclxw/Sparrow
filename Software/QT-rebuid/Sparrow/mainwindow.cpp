#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "driver_usb.h"
#include "interface/usblistener.h"
#include "system.h"

Driver_Usb *usbDriver;
usblistener *listener = Q_NULLPTR;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(this->width(),this->height());


    listener = new usblistener;

    qApp -> installNativeEventFilter(listener);
    usbDriver = new Driver_Usb;

//    QList<QStorageInfo> msg;
//    sysGetDiskMsg(&msg);

    connect(listener, &usblistener::DevicePlugIn, usbDriver, &Driver_Usb::usbConnectDevice);
}

MainWindow::~MainWindow()
{
    delete ui;
}

