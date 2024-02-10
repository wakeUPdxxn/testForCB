#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QException>
#include <QImageReader>
#include <QtConcurrent>
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
    QThread *dbThread;
    QFuture<void>imageQueueProccessing;

    std::atomic<bool>isFMimageProccesed=false;
    std::atomic<bool>isMWimageProccesed=false;
    QQueue<QPixmap*>imageProccesingQueue;

private:
    void makeSetUp();
    void waitForImageProccesed();

private slots:
    void connectionHandler();

signals:
    void WriteToDb(const QString imageName, const QString path, const QString date);
    void ReadFromDb();
    void newImage(const QPixmap *image,const QString name);

public slots:
    void disconnectionEvent();
    void onServerReadyRead();
    void getDBdata();
    void setFMimageProccesed();
    void setMVimageProccesed();
};

