#pragma once
#include <QObject>
#include <QDir>

class LocalDataManager : public QObject
{
    Q_OBJECT
public:
    explicit LocalDataManager(QObject *parent = nullptr);
    void saveImage(const QPixmap &image,const QString name,const QString format);
private:
    QDir rootFolder;
    const QString path = "./Downloads";
signals:

};

