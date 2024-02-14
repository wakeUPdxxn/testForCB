#include "localdatamanager.h"
#include <QJsonDocument>
#include <QJsonObject>

LocalDataManager::LocalDataManager(QObject *parent)
    : QObject{parent}
{
    if(!QFile::exists(storagePath)){
        rootFolder.mkdir(storagePath);
    }
}

void LocalDataManager::onNewImage(const QPixmap *image,const QString name)
{
    QImage img = image->toImage();
    if(!img.save(storagePath+name)){
        qDebug() << "img saving error";
    }
    emit fmImageProcessed();
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
    QFile config{"./configs/dbConf.json"};
    config.open(QIODevice::ReadOnly);  
    QJsonObject obj = QJsonDocument::fromJson(config.readAll()).object();
    config.close();
    if(obj.isEmpty()){
       config.open(QIODevice::WriteOnly);

       QVariantMap data;
       data["dbName"]="./testDb.db";
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

std::tuple<QHostAddress, quint16, QString> LocalDataManager::getConfigData()
{
    QFile config{"./configs/config.json"};
    config.open(QIODevice::ReadOnly);
    QJsonObject obj=QJsonDocument::fromJson(config.readAll()).object();
    config.close();
    if(obj.isEmpty()){
       config.open(QIODevice::WriteOnly);

       QVariantMap data;
       data["listetingAddress(es)"]="localhost";
       data["port"]="2323";
       data["downloadFolderPath"]="./Downloads/";

       config.write(QJsonDocument::fromVariant(data).toJson());
       config.close();

       return getConfigData();
    }
    else{
        return {QHostAddress(obj["listetingAddress(es)"].toString()),obj["port"].toString().toUInt(),obj["downloadFolderPath"].toString()};
    }
}
