#include "dialog_ble_name.h"
#include "ui_dialog_ble_name.h"
#include "driver_usb.h"
#include "mainwindow.h"

Dialog_Ble_Name::Dialog_Ble_Name(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Ble_Name)
{
    ui->setupUi(this);
}

Dialog_Ble_Name::~Dialog_Ble_Name()
{
    delete ui;
}

void Dialog_Ble_Name::on_btnSaveBleName_clicked()
{
    uint8_t *bytesBleName;
    QByteArray baBleName;

    QString bleName = ui->leBleName->text();
    uint8_t bleNameLen = bleName.size() + 1;

    baBleName = bleName.toLatin1();
    bytesBleName = (uint8_t*)baBleName.data();

    usbDriver->usbSetBleName(bytesBleName, bleNameLen);
}

