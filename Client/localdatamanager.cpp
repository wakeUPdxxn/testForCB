#include "localdatamanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>

LocalDataManager::LocalDataManager(){}

void LocalDataManager::saveConnectionData(const QHostAddress ip, const qint16 port, const bool flag)
{
    QFile userData{QApplication::applicationDirPath()+"/connectionData.json"};
    userData.open(QIODevice::ReadOnly);
    QJsonObject obj=QJsonDocument::fromJson(userData.readAll()).object();
    userData.close();
    if(!ip.isNull()){
        obj["address"]=ip.toString();
    }
    if(port!=0){
        obj["port"]=port;
    }
    if(obj["autoConnect"].toBool()!=flag){
        obj["autoConnect"]=flag;
    }
    userData.open(QIODevice::WriteOnly);
    userData.write(QJsonDocument(obj).toJson());
    userData.close();
}

std::tuple<QHostAddress,quint16,bool> LocalDataManager::getConnectionData()
{
    QFile userData{QApplication::applicationDirPath()+"/connectionData.json"};
    userData.open(QIODevice::ReadOnly);
    QJsonObject obj=QJsonDocument::fromJson(userData.readAll()).object();
    userData.close();
    if(obj.empty()){
        userData.open(QIODevice::WriteOnly);

        QVariantMap data;
        data["address"]="";
        data["port"]=0;
        data["autoConnect"]=false;

        userData.write(QJsonDocument::fromVariant(data).toJson());
        userData.close();

       return getConnectionData();
    }
    else{
        return {QHostAddress(obj["address"].toString()),obj["port"].toInt(),obj["autoConnect"].toBool()};
    }
}

