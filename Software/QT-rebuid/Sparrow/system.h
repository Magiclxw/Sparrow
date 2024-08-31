#ifndef SYSTEM_H
#define SYSTEM_H


#include <QFile>
#include <QDataStream>
#include <QStorageInfo>

bool sysSaveDataToFile(QString fileName, uint8_t data[], int dataLen);
void sysGetDiskMsg(QList<QStorageInfo> *msg);
void sysGetNetSpeed(uint64_t* downLoadSpeed, uint64_t* uploadSpeed);
double sysGetCpuInfo();
uint8_t sysGetMemeryUsage();

#endif // SYSTEM_H
