#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QException>
#include <QImageReader>
#include <QtConcurrent>
#include <QMutex>
#include "dbhandler.h"
#include "localdatamanager.h"
#include "mainwindow.h"

class Backend : public QTcpServer
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

private:
    MainWindow *m_mainWindow;//убрать из этого класса вообще и сделать backend friend в window
    QList<QTcpSocket*> clients;
    DBhandler *m_dbHandler;
    LocalDataManager *m_dataManager;
    QTcpSocket *m_clientSock;
    qint64 messageSize{0};
    QByteArray data;
    QThread *dbThread;
    QThread *fmThread;
    QFuture<void>imageQueueProcessing;

    QMutex m_mt;
    std::atomic<bool>isFMimageProcessed=false;
    QQueue<QPixmap*>imageProcessingQueue;

private:
    void makeSetUp();
    void waitForImageProcesed();

private slots:
    void connectionHandler();

signals:
    void WriteToDb(const QString imageName, const QString path, const QString date);
    void ReadFromDb();
    void newImage(const QPixmap *image,const QString name);

public slots:
    void disconnectionEvent();
    void onServerReadyRead();
    void setFMimageProcessed();
};

