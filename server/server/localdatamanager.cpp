#include "localdatamanager.h"

LocalDataManager::LocalDataManager(QObject *parent)
    : QObject{parent}
{
    if(!QFile::exists(root)){
        QDir rootFolder("../");
        rootFolder.mkdir("Downloads");
    }
}
