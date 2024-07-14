#ifndef FORM_ENTRANCE_H
#define FORM_ENTRANCE_H

#include <QWidget>
#include "usb_listener.h"

extern USB_Listener *usbListener;

namespace Ui {
class Form_Entrance;
}

class Form_Entrance : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Entrance(QWidget *parent = nullptr);
    ~Form_Entrance();

signals:
    void signalDeviceIn(QString VID, QString PID);

private:
    Ui::Form_Entrance *ui;
private slots:
    void onDeviceIn(QString VID, QString PID);
    void onDeviceOut(QString VID, QString PID);
};

#endif // FORM_ENTRANCE_H
