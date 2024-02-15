#include "localdatamanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>

LocalDataManager::LocalDataManager(QObject *parent)
    : QObject{parent}
{
    rootFolder.setPath(QCoreApplication::applicationDirPath());
}

LocalDataManager::LocalDataManager(const QString &storagePath)
{
    rootFolder.setPath(QCoreApplication::applicationDirPath());
    this->setStoragePath(storagePath);
}

void LocalDataManager::onNewImage(const QPixmap *image,const QString name)
{;
    QImage img = image->toImage();
    if(!img.save(storageDir.absolutePath()+"/"+name)){
        qDebug() << "img saving error";
    }
    emit fmImageProcessed();
}

QString LocalDataManager::getStoragePath() const
{
    return storageDir.absolutePath();
}

void LocalDataManager::setStoragePath(const QString &path)
{  
    if(!QFile::exists(path)) { //≈сли указанна€ в конфиге директори€ отсутствует, то она будет создана автоматически
        storageDir.mkdir(path);
    }
    else{
        storageDir.cd(path);
    }
}

uint LocalDataManager::GetImagesCount()
{
    return storageDir.count()-2;
}

void LocalDataManager::saveDbData(const QString name, const QString host, const std::optional<QString> user, const std::optional<QString> password, const QString driver)
{
    QFile config{"./configs/dbConf.json"};
    config.open(QIODevice::WriteOnly);
    QVariantMap data;

    data["dbName"]=name;

    if(host.isEmpty()){
        data["hostName"]="localhost";
    }
    else{
        data["hostName"]=host;
    }

    data["user"]=user.value_or("");
    data["password"]=password.value_or("");

    if(driver.isEmpty()){
        data["dbDriverName"]="QSQLITE";
    }
    else{
        data["dbDriverName"]=driver;
    }

    QJsonDocument jdoc=QJsonDocument::fromVariant(data);
    config.write(jdoc.toJson());
    config.close();
}

QVariantMap LocalDataManager::getDbData()
{
    QFile config{QCoreApplication::applicationDirPath()+"/configs/dbConf.json"};
    config.open(QIODevice::ReadOnly);  
    QJsonObject obj = QJsonDocument::fromJson(config.readAll()).object();
    config.close();
    if(obj.isEmpty()){
       config.open(QIODevice::WriteOnly);

       QVariantMap data;
       data["dbName"]=QCoreApplication::applicationDirPath()+"/testDb.db";
       data["hostName"]="localhost";
       data["user"]="";
       data["password"]="";
       data["dbDriverName"]="QSQLITE";

       config.write(QJsonDocument::fromVariant(data).toJson());
       config.close();

       return getDbData();
    }
    else{
        return obj.toVariantMap();
    }
}

std::tuple<QString, quint16, QString> LocalDataManager::getConfigData()
{
    if(!QFile::exists(QCoreApplication::applicationDirPath()+"/configs")){
       QDir(QCoreApplication::applicationDirPath()).mkdir("configs");
    }

    QFile config{QCoreApplication::applicationDirPath()+"/configs/config.json"};
    config.open(QIODevice::ReadOnly);

    QJsonObject obj=QJsonDocument::fromJson(config.readAll()).object();

    config.close();

    if(obj.isEmpty()){
       config.open(QIODevice::WriteOnly);

       QVariantMap data;
       data["listetingAddress(es)"]=QHostAddress::LocalHost;
       data["port"]="2323";
       data["downloadFolderPath"]=QCoreApplication::applicationDirPath()+"/Downloads";

       config.write(QJsonDocument::fromVariant(data).toJson());
       config.close();

       return getConfigData();
    }
    else{
        return {obj["listetingAddress(es)"].toString(),obj["port"].toString().toUInt(),obj["downloadFolderPath"].toString()};
    }
}




