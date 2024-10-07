#include "dialog_bilibili_cfg.h"
#include "ui_dialog_bilibili_cfg.h"
#include "driver_usb.h"
#include "mainwindow.h"


Dialog_Bilibili_Cfg::Dialog_Bilibili_Cfg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Bilibili_Cfg)
{
    ui->setupUi(this);
}

Dialog_Bilibili_Cfg::~Dialog_Bilibili_Cfg()
{
    delete ui;
}

void Dialog_Bilibili_Cfg::on_btnSetBilibili_clicked()
{
    uint8_t *bytesBilibili;
    QByteArray baBilibili;

    QString bilibili = ui->leSetBilibili->text();
    uint8_t bilibiliLen = bilibili.size() + 1;

    baBilibili = bilibili.toLatin1();
    bytesBilibili = (uint8_t*)baBilibili.data();

    usbDriver->usbSetBilibiliUrl(bytesBilibili, bilibiliLen);
}

