#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "driver_usb.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern Driver_Usb *usbDriver;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_btnSetWifi_clicked();

    void on_pushButton_3_clicked();

    void on_cbPCMonitor_stateChanged(int arg1);

signals:
    void signalSetPcMonitor(uint8_t key);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H