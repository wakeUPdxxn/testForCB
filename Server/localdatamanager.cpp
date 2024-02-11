#include "localdatamanager.h"

LocalDataManager::LocalDataManager(QObject *parent)
    : QObject{parent}
{
    if(!QFile::exists(rootPath)){
        rootFolder.mkdir(rootPath);
    }
}

void LocalDataManager::saveImage(const QPixmap *image,const QString name)
{
    QImage img = image->toImage();
    if(!img.save(rootPath+name)){
        qDebug() << "img saving error";
    }
    emit ImageProccessed();
}

QString LocalDataManager::getRootPath() const
{
    return rootPath;\
}

void LocalDataManager::setRootPath(const QString &path)
{
    this->rootPath=path;
}

uint LocalDataManager::GetImagesCount()
{
    return rootFolder.count();
}
