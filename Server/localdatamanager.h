#pragma once
#include <QObject>
#include <QDir>
#include <QImage>

class LocalDataManager : public QObject
{
    Q_OBJECT
public:
    explicit LocalDataManager(QObject *parent = nullptr);
    void saveImage(const QImage &image,const QString name) const;
    QString getRootPath() const;
private:
    QDir rootFolder;
    const QString path = "./Downloads";
signals:

};

