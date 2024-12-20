#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "interface/usblistener.h"
#include "system.h"
#include <QDebug>
#include "dialog_wifi_cfg.h"
#include "dialog_mqtt_cfg.h"
#include "dialog_weather_cfg.h"
#include "dialog_bilibili_cfg.h"
#include "dialog_ble_name.h"
#include "dialog_sleep_time.h"
#include <QFileIconProvider>
#include <QLabel>
#include <QProcess>
#include <QFileDialog>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QTimer>

Driver_Usb *usbDriver;
usblistener *listener = Q_NULLPTR;
QTimer *recTextRefreshTimer = NULL;   //文本接收定时器

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Sparrow");
    this->setWindowIcon(QIcon(":/icon/Icon.png"));
    this->setFixedSize(this->width(),this->height());

    listener = new usblistener;

    qApp -> installNativeEventFilter(listener);
    usbDriver = new Driver_Usb;
    usbDriver->start();

    QString pcMonitor = sysGetCfg("pcMonitor");
    if (pcMonitor == "true")
    {
        ui->cbPCMonitor->setCheckState(Qt::Checked);
        usbDriver->usbPcMonitorCtrl(1);
    }

    recTextRefreshTimer = new QTimer;

    recTextRefreshTimer->start(100);

//    QList<QStorageInfo> msg;
//    sysGetDiskMsg(&msg);

    connect(listener, &usblistener::DevicePlugIn, usbDriver, &Driver_Usb::usbConnectDevice);
    connect(recTextRefreshTimer,&QTimer::timeout,this,&MainWindow::setText);
    updateRecFileList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

uint8_t generateChecksum(uint8_t data[], uint8_t len)
{
    uint8_t checkSum = 0;

    for(uint8_t i = 0; i < len; i++)
    {
        checkSum += data[i];
    }

    return checkSum;
}


int sendCdcData(uint8_t cmd, uint8_t data[], uint16_t dataLen)
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
    sendData[dataLen+5+2] = SERIAL_PROTOCOL_STOP_L;

    serial.write((char*)sendData, dataLen + 8);

    return 1;
}

void MainWindow::on_pushButton_clicked()
{
    QList<QStorageInfo> msg;
    sysGetDiskMsg(&msg);
    uint8_t diskNum = msg.count();
    uint8_t *data = (uint8_t*)malloc(diskNum * 2 + 1); //1:diskNum
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
        diskPath =msg.value(i).rootPath().toUtf8().at(0); //msg.takeAt(i).rootPath().toUtf8().at(0);

        diskSize =msg.value(i).bytesTotal() / (1024 * 1024);
        remainSize = msg.value(i).bytesAvailable() / (1024 * 1024);
        data[1 + i * 2] = diskPath;
        data[1 + i * 2 + 1] = (uint8_t)(100 - ((float)remainSize/(float)diskSize)*100);

    }
    sendCdcData(SERIAL_CMD_DISK_INFO, data, 1 + diskNum * 2);
    //sysGetNetSpeed();
//    sysGetCpuInfo();
    qDebug() << "send disk info";

    //free(data);
}


void MainWindow::on_btnSetWifi_clicked()
{
    Dialog_Wifi_CFG dialog(this);

    dialog.exec();
}

void MainWindow::on_btnSetMqtt_clicked()
{
    Dialog_Mqtt_cfg dialog(this);

    dialog.exec();
}

void MainWindow::on_btnSetWeather_clicked()
{
    Dialog_Weather_Cfg dialog(this);

    dialog.exec();
}

void MainWindow::on_btnSetBilibili_clicked()
{
    Dialog_Bilibili_Cfg dialog(this);

    dialog.exec();
}


void MainWindow::on_cbPCMonitor_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        sysSaveCfg("pcMonitor", "true");
        usbDriver->usbPcMonitorCtrl(1);
    }
    else if (arg1 == Qt::Unchecked)
    {
        sysSaveCfg("pcMonitor", "false");
        usbDriver->usbPcMonitorCtrl(0);
    }
}


void MainWindow::on_btnSleepTime_clicked()
{
    Dialog_Sleep_Time dialog(this);

    dialog.exec();
}

void MainWindow::updateRecFileList()
{
    QStringList fileList = getFileNames();
    uint16_t fileNum = fileList.count();

//    qDebug() << "file number = " << fileNum;


    for(int i = 0; i < fileNum; i++)
    {
        QListWidgetItem *listwidgetitem = new QListWidgetItem;
        QFileInfo fileInfo(fileFolder+"/"+fileList.value(i));

        QFileIconProvider iconProvider;
        QString fileName = fileInfo.fileName();
        QIcon icon = iconProvider.icon(fileInfo);
        QLabel label;
        label.setPixmap(icon.pixmap(50,50));

        listwidgetitem->setIcon(icon);
        listwidgetitem->setText(fileName);

        ui->lwRecFiles->addItem(listwidgetitem);
    }
}

void MainWindow::on_lwRecFiles_itemDoubleClicked(QListWidgetItem *item)
{
    uint16_t index = 0;

    //获取被选中行
    index = ui->lwRecFiles->currentIndex().row();

    QString name = ui->lwRecFiles->currentIndex().data().toString();

    QFileInfo fileInfo(name);

    QString path = fileInfo.absoluteFilePath();

    qDebug() << "clicked : " << path;

    QProcess::startDetached(path);


}

void MainWindow::on_btnOpenFloader_clicked()
{

    QString path = QDir::currentPath();

    QDesktopServices::openUrl(path + "/" +fileFolder);
}

void MainWindow::setText()
{
    char * text;
    if (usbDriver->usbGetTextRecFinishFlag() == 1)
    {
        text = (char*)usbDriver->usbGetRecText();
        QString data = QString::fromUtf8(reinterpret_cast<const char*>(text), usbDriver->usbGetTextRecDataLen());
        ui->teRecText->setText(text);

        usbDriver->usbSetTextRecFinishFlag(0);
    }

}

void MainWindow::on_btnSetBleName_clicked()
{
    Dialog_Ble_Name dialog(this);
    dialog.exec();
}

