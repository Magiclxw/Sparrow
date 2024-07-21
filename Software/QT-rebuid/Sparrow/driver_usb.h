#ifndef DRIVER_USB_H
#define DRIVER_USB_H

#include <QThread>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#define USB_VID  (0x303A)
#define USB_PID  (0x4005)

#define SERIAL_PROTOCOL_START_H    (0xC0)
#define SERIAL_PROTOCOL_START_L    (0x0C)
#define SERIAL_PROTOCOL_STOP_H     (0xED)
#define SERIAL_PROTOCOL_STOP_L     (0xDE)

#define SERIAL_CMD_FILE_START      (0x03)
#define SERIAL_CMD_FILE            (0x04)

class Driver_Usb : public QThread
{
public:
    explicit Driver_Usb(QObject *parent = nullptr);
    void usbConnectDevice();
    void run();
private:

    void serialReadData();
    void handleSerialError(QSerialPort::SerialPortError error);
};

#endif // DRIVER_USB_H
