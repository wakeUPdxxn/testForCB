#include "localdatamanager.h"

LocalDataManager::LocalDataManager(QObject *parent)
    : QObject{parent}
{
    if(!QFile::exists(path)){
        rootFolder.mkdir(path);
    }
}

void LocalDataManager::saveImage(const QImage &image,const QString name) const
{
    image.save(name);
}

QString LocalDataManager::getRootPath() const
{
    return rootFolder.path();
}
