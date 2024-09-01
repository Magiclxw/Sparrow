#include "dialog_wifi_cfg.h"
#include "ui_dialog_wifi_cfg.h"
#include "mainwindow.h"
#include "driver_usb.h"

Dialog_Wifi_CFG::Dialog_Wifi_CFG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Wifi_CFG)
{
    ui->setupUi(this);

    //connect(ui->btnWifiSave, &QPushButton::clicked,usbDriver, &Driver_Usb::usbSetWifiInfo);
}

Dialog_Wifi_CFG::~Dialog_Wifi_CFG()
{
    delete ui;
}

void Dialog_Wifi_CFG::on_btnWifiSave_clicked()
{
    uint8_t *bytesSsid;
    uint8_t *bytesPassword;
    QByteArray baSsid;
    QByteArray baPassword;

    QString ssid = ui->leSsid->text();
    uint8_t ssidLen = ssid.size() + 1;
    QString password = ui->lePassword->text();
    uint8_t passwordLen = password.size() + 1;


    baSsid = ssid.toLatin1();
    bytesSsid = (uint8_t*)baSsid.data();

    baPassword = password.toLatin1();
    bytesPassword = (uint8_t*)baPassword.data();

    usbDriver->usbSetWifiInfo(bytesSsid, ssidLen, bytesPassword, passwordLen);
}

