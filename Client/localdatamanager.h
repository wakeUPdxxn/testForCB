#pragma once
#include <QFile>
#include <QHostAddress>

class LocalDataManager
{
public:
    explicit LocalDataManager();
    ~LocalDataManager() = default;
    static void saveConnectionData(const QHostAddress ip,const qint16 port,const bool flag);
    static std::tuple<QHostAddress,quint16,bool> getConnectionData();
};


