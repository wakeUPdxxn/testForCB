#include <QObject>
#include <QTcpSocket>
#include "mainwindow.h"
#include <QThread>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
    void makeSetUp();
private:
    MainWindow *m_mainWindow{nullptr};
    QTcpSocket *m_socket{nullptr};

signals:
    void disableUiBlock();

private slots:
    void responseReceived();
    void sendImage(const QImage *image,const QString name);
    void connectToServer(const QHostAddress serverAddr);
};

