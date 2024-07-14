#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QFile>
#include <QDataStream>

//class FileManager
//{
//public:
//    FileManager();

//};

bool saveDataToFile( QString &filePath, uint8_t data[], int dataLen);

#endif // FILEMANAGER_H
