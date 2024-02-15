#pragma once
#include <QObject>
#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QHostAddress>

class LocalDataManager : public QObject
{
    Q_OBJECT
public:
    explicit LocalDataManager(QObject *parent = nullptr);

    LocalDataManager(const QString &storagePath);

    ~LocalDataManager()=default;

    QString getStoragePath() const;
    void setStoragePath(const QString &path);
    uint GetImagesCount();
    static void saveDbData(const QString name,const QString host,const std::optional<QString> user,const std::optional<QString> password,const QString driver);
    static QVariantMap getDbData();
    static std::tuple<QString, quint16, QString> getConfigData();

private:
    QDir rootFolder;
    QDir storageDir;

public slots:
    void onNewImage(const QPixmap *image,const QString name);
signals:
    void fmImageProcessed();

};

