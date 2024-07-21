#ifndef USBLISTENER_H
#define USBLISTENER_H

#include <QWidget>
#include <windows.h>
#include <QAbstractNativeEventFilter>
#include <dbt.h>

class usblistener:public QWidget, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    void EmitMySignal();

protected:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

signals:
    void DeviceChangeCbk();
    void DevicePlugIn();
    void DevicePlugOut();
};

#endif // USBLISTENER_H
