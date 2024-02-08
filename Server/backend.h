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
    QThread *dbThread;
    QThread *fmThread;
    QImageReader imr;

private:
    void makeSetUp();

private slots:
    void connectionHandler();

signals:
    void WriteToDb(const QString imageName, const QString path, const QString date);
    void ReadFromDb();
    void newImage(QImage &image,const QString name);

public slots:
    void disconnectionEvent();
    void imageReceived();
    void getDBdata();
};

