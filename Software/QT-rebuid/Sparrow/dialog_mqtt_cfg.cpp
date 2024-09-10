#include "dialog_mqtt_cfg.h"
#include "ui_dialog_mqtt_cfg.h"
#include "mainwindow.h"

Dialog_Mqtt_cfg::Dialog_Mqtt_cfg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Mqtt_cfg)
{
    ui->setupUi(this);
}

Dialog_Mqtt_cfg::~Dialog_Mqtt_cfg()
{
    delete ui;
}


void Dialog_Mqtt_cfg::on_btnMqttSaveAddr_clicked()
{
    QString addr = ui->leMqttAddr->text();
    QByteArray baAddr;
    uint8_t addrLen = addr.size()+1;    //+1 结束符
    uint8_t* bytesAddr;

    baAddr = addr.toLatin1();
    bytesAddr = (uint8_t*)baAddr.data();

    usbDriver->usbSetMqttInfo(MQTT_DATA_ADDR,bytesAddr, addrLen);
}


void Dialog_Mqtt_cfg::on_btnMqttSaveUsername_clicked()
{
    QString username = ui->leMqttUserName->text();
    QByteArray baUsername;
    uint8_t usernameLen = username.size()+1;
    uint8_t* bytesUsername;

    baUsername = username.toLatin1();
    bytesUsername = (uint8_t*)baUsername.data();

    usbDriver->usbSetMqttInfo(MQTT_DATA_USERNAME,bytesUsername, usernameLen);
}



void Dialog_Mqtt_cfg::on_btnMqttSavePassword_clicked()
{
    QString password = ui->leMqttPassword->text();
    QByteArray baPassword;
    uint8_t passwordLen = password.size()+1;
    uint8_t* bytesPassword;

    baPassword = password.toLatin1();
    bytesPassword = (uint8_t*)baPassword.data();

    usbDriver->usbSetMqttInfo(MQTT_DATA_PASSWORD,bytesPassword, passwordLen);
}

