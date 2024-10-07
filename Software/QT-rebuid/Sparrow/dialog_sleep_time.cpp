#include "dialog_sleep_time.h"
#include "ui_dialog_sleep_time.h"
#include "mainwindow.h"

Dialog_Sleep_Time::Dialog_Sleep_Time(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Sleep_Time)
{
    ui->setupUi(this);
}

Dialog_Sleep_Time::~Dialog_Sleep_Time()
{
    delete ui;
}

void Dialog_Sleep_Time::on_btnSaveSleepTime_clicked()
{
    uint32_t sleepTime = 0;

    sleepTime = ui->sbSleepHour->value()*3600 + ui->sbSleepMinute->value()*60;

    usbDriver->usbSetSleepTime(sleepTime);
}

