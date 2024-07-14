#include "filemanager.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>

bool saveDataToFile( QString &filePath, uint8_t data[], int dataLen)
{
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream  out(&file);

    //QByteArray byte_array = QByteArray::fromRawData((const char*)data, dataLen);

    for (int i = 0; i < dataLen; i++)
    {
        out << data[i];
    }

    //file.rename("")

    file.close();
    return true;
}
