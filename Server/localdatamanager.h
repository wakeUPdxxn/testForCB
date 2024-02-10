#pragma once
#include <QObject>
#include <QDir>
#include <QImage>
#include <QPixmap>

class LocalDataManager : public QObject
{
    Q_OBJECT
public:
    explicit LocalDataManager(QObject *parent = nullptr);
    ~LocalDataManager()=default;
    void saveImage(const QPixmap *image,const QString name);
    QString getRootPath() const;
    void setRootPath(const QString &path);

private:
    QDir rootFolder;
    QString path = ".\\Downloads\\";

signals:
    void ImageProccessed();

};

