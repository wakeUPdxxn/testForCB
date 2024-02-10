#include "localdatamanager.h"

LocalDataManager::LocalDataManager(QObject *parent)
    : QObject{parent}
{
    if(!QFile::exists(path)){
        rootFolder.mkdir(path);
    }
}

void LocalDataManager::saveImage(const QPixmap *image,const QString name)
{
    QImage img = image->toImage();
    if(!img.save(path+name)){
        qDebug() << "img saving error";
    }
    emit ImageProccessed();
}

QString LocalDataManager::getRootPath() const
{
    return rootFolder.path();
}

void LocalDataManager::setRootPath(const QString &path)
{
    this->path=path;
}
