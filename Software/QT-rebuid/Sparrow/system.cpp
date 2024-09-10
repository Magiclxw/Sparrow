#include "system.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QDir>
#include <QDebug>
#include <iphlpapi.h>
#include <winsock.h>
#include <iostream>
#include <QProcess>
#include <psapi.h>
#include <Windows.h>
#include <winioctl.h>
#include <QSettings>

QString fileFolder = "file";
QString sysConfig = "config/systemConfig.ini";

//上传速度
static DWORD dwIn = 0;
//下载速度
static DWORD dwOut = 0;
//上传最后字节
static DWORD dwLastIn = 0;
//下载最后字节
static DWORD dwLastOut = 0;

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

void sysGetDiskMsg(QList<QStorageInfo> *msg)
{
    // 获取所有挂载的磁盘信息
    *msg = QStorageInfo::mountedVolumes();

    // 打印磁盘个数
    qDebug() << "磁盘个数：" << msg->count();

    // 打印每个磁盘的信息
    for (const QStorageInfo &disk : *msg) {
        qDebug() << "磁盘路径：" << disk.rootPath();
        qDebug() << "磁盘文件系统：" << disk.fileSystemType();
        qDebug() << "磁盘总大小：" << disk.bytesTotal() / (1024 * 1024) << "MB";
        qDebug() << "磁盘可用空间：" << disk.bytesAvailable() / (1024 * 1024) << "MB";
        qDebug() << "-----------------------";
    }
}

void sysGetNetSpeed(uint64_t* downLoadSpeed, uint64_t* uploadSpeed)
{
    PMIB_IFTABLE pTable = nullptr;
    DWORD dword = 0;
    ULONG retCode = GetIfTable(pTable, &dword, true);
    if(retCode == ERROR_NOT_SUPPORTED)
        return;
    if(retCode == ERROR_INSUFFICIENT_BUFFER)
        pTable = (PMIB_IFTABLE)new BYTE[65535];


    GetIfTable(pTable, &dword, true);

    DWORD dwInOc = 0;
    DWORD dwOutOc = 0;
    for(UINT i = 0; i < pTable->dwNumEntries; i++)
    {
        MIB_IFROW row = pTable->table[i];
        dwInOc += row.dwInOctets;
        dwOutOc += row.dwOutOctets;
    }
    dwIn = dwInOc - dwLastIn;
    dwOut = dwOutOc - dwLastOut;

    if(dwLastIn <= 0)
        dwIn = 0;
    else
        dwIn = dwIn / 1024;

    if(dwLastOut <= 0)
        dwOut = 0;
    else
        dwOut = dwOut / 1024;

    dwLastIn = dwInOc;
    dwLastOut = dwOutOc;

    qDebug() << "upload speed = " + QString::number(dwOut);
    qDebug() << "download speed = " + QString::number(dwIn);

    *downLoadSpeed = dwIn/8;
    *uploadSpeed = dwOut/8;
}

double nCpuRate = 0;

uint8_t sysGetMemeryUsage()
{
    QString memoryInfo = "Memory Information:\n";

    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);

    float memery = 0;


     if (GlobalMemoryStatusEx(&memoryStatus)) {
//         memoryInfo+=QString("Total Physical Memory: %1 %2\n").arg(memoryStatus.ullTotalPhys / (1024 * 1024)).arg("MB");
//         memoryInfo+=QString("Available Physical Memory: %1 %2\n").arg(memoryStatus.ullAvailPhys / (1024 * 1024)).arg("MB");
//         memoryInfo+=QString("Total Virtual Memory: %1 %2\n").arg(memoryStatus.ullTotalVirtual / (1024 * 1024)).arg("MB");
//         memoryInfo+=QString("Available Virtual Memory: %1 %2\n").arg(memoryStatus.ullAvailVirtual / (1024 * 1024)).arg("MB");
         memery = (float)((float)memoryStatus.ullAvailPhys / (float)memoryStatus.ullTotalPhys);
     } else {
         memoryInfo+=QString("无法获取内存使用情况信息。\n");
     }
     qDebug() << memoryInfo;

     return (uint8_t)(100 - memery*100);
}

void sysSaveCfg(QString key, QString value)
{
    QSettings *iniWrite = new QSettings(sysConfig,QSettings::IniFormat);
    iniWrite->setValue(key, value);
}

QString sysGetCfg(QString key)
{
    QFile file(sysConfig);

    QString data;

    if(file.open(QIODevice::ReadOnly)){    //文件存在
        QSettings *iniRead = new QSettings(sysConfig, QSettings::IniFormat);
        data = iniRead->value(key, 0).toString();
    }

    return data;
}
