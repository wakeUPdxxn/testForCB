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
    QString getStoragePath() const;
    void setStoragePath(const QString &path);
    uint GetImagesCount();

private:
    QDir rootFolder;
    QString storagePath = ".\\Downloads\\";

signals:
    void ImageProccessed();

};

