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
    QString getStoragePath() const;
    void setStoragePath(const QString &path);
    uint GetImagesCount();

private:
    QDir rootFolder;
    QString storagePath = ".\\Downloads\\";

public slots:
    void onNewImage(const QPixmap *image,const QString name);

signals:
    void fmImageProcessed();

};

