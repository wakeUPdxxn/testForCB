#include "localdatamanager.h"
#include "qpixmap.h"

LocalDataManager::LocalDataManager(QObject *parent)
    : QObject{parent}
{
    if(!QFile::exists(path)){
        rootFolder.mkdir(path);
    }
}

void LocalDataManager::saveImage(const QPixmap &image,const QString name,const QString format)
{
    QFile file(path+name);
    file.open(QIODevice::WriteOnly);
    image.save(&file,format.toStdString().c_str());
    file.close();
}
