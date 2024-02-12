#include "localdatamanager.h"

LocalDataManager::LocalDataManager(QObject *parent)
    : QObject{parent}
{
    if(!QFile::exists(storagePath)){
        rootFolder.mkdir(storagePath);
    }
}

void LocalDataManager::saveImage(const QPixmap *image,const QString name)
{
    QImage img = image->toImage();
    if(!img.save(storagePath+name)){
        qDebug() << "img saving error";
    }
    emit ImageProccessed();
}

QString LocalDataManager::getStoragePath() const
{
    return storagePath;
}

void LocalDataManager::setStoragePath(const QString &path)
{
    this->storagePath=path;
}

uint LocalDataManager::GetImagesCount()
{
    rootFolder.cd(storagePath);
    return rootFolder.count()-2;
}
