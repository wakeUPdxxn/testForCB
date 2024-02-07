#pragma once
#include <QObject>
#include <QDir>

class LocalDataManager : public QObject
{
    Q_OBJECT
public:
    explicit LocalDataManager(QObject *parent = nullptr);
    void createFolder(const QString &name);
    void writeIntoFolder(const QString &name);
private:
    const QString root = "../Downloads";
signals:

};

