#ifndef SYSTEM_H
#define SYSTEM_H


#include <QFile>
#include <QDataStream>
#include <QStorageInfo>

bool sysSaveDataToFile(QString fileName, uint8_t data[], int dataLen);
void sysGetDiskMsg(QList<QStorageInfo> *msg);

#endif // SYSTEM_H
