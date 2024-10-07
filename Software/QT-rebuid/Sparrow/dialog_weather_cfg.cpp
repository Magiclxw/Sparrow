#include "dialog_weather_cfg.h"
#include "ui_dialog_weather_cfg.h"
#include "driver_usb.h"
#include "mainwindow.h"

Dialog_Weather_Cfg::Dialog_Weather_Cfg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Weather_Cfg)
{
    ui->setupUi(this);
}

Dialog_Weather_Cfg::~Dialog_Weather_Cfg()
{
    delete ui;
}

void Dialog_Weather_Cfg::on_btnSetWeather_clicked()
{
    uint8_t *bytesWeather;
    QByteArray baWeather;

    QString weather = ui->leSetWeather->text();
    uint8_t weatherLen = weather.size() + 1;

    baWeather = weather.toLatin1();
    bytesWeather = (uint8_t*)baWeather.data();

    usbDriver->usbSetWeatherUrl(bytesWeather, weatherLen);
}

