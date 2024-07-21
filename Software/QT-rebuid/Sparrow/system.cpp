#include "system.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QDir>
#include <QDebug>

QString fileFolder = "file";

bool sysSaveDataToFile(QString fileName, uint8_t data[], int dataLen)
{
    QDir dir(QDir::currentPath());


    if(!dir.exists(fileFolder))    //如果Images文件夹不存在
    {
        dir.mkdir(fileFolder);    //创建文件夹（名为Images）

        qDebug()<<QString("文件夹%1创建成功！").arg(fileFolder);
    }
    else
    {
        qDebug()<<QString("文件夹%1已存在！").arg(fileFolder);
    }

    QFile file(fileFolder + "/" + fileName);

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
