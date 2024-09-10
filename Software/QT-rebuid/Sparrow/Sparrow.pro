QT       += core gui
QT  += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog_mqtt_cfg.cpp \
    dialog_wifi_cfg.cpp \
    driver_usb.cpp \
    interface/jqcpumonitor.cpp \
    interface/usblistener.cpp \
    main.cpp \
    mainwindow.cpp \
    system.cpp

HEADERS += \
    dialog_mqtt_cfg.h \
    dialog_wifi_cfg.h \
    driver_usb.h \
    interface/jqcpumonitor.h \
    interface/jqcpumonitor.inc \
    interface/usblistener.h \
    libs/hidapi.h \
    mainwindow.h \
    system.h

FORMS += \
    dialog_mqtt_cfg.ui \
    dialog_wifi_cfg.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

DISTFILES += \
    libs/hidapi.dll \
    libs/hidapi.lib \
    libs/iphlpapi.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -liphlpapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -liphlpapi
else:unix: LIBS += -L$$PWD/libs/ -liphlpapi

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs
