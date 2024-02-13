#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QtConcurrent>
#include <QPixmap>
#include <QFileInfo>
#include "localdatamanager.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
    void makeSetUp();

private:
    QTcpSocket *m_socket{nullptr};
    QByteArray data;  
    LocalDataManager dm;
    QHostAddress serverAddr;
    quint16 port;
    bool autoConnect;
    QRegularExpression ipRex{"([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})"};

signals:
    void disableUiBlock();
    void setMessage(const QString &title,const QString &text,const QString &type);
    void imageSent();
    void changeStatus(const int);

public slots:
    void responseReceived();
    void onSendImage(const QPixmap *image,const QString name);
    void connectToServer();
    void setConnectionData();
};

