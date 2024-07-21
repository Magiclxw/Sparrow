#ifndef SYSTEM_H
#define SYSTEM_H


#include <QFile>
#include <QDataStream>


bool sysSaveDataToFile(QString fileName, uint8_t data[], int dataLen);

#endif // SYSTEM_H
