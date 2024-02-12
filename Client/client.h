#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QtConcurrent>
#include <QPixmap>
#include <QFileInfo>

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

signals:
    void disableUiBlock();
    void setMessage(const QString &title,const QString &text,const QString &type);
    void imageSent();

public slots:
    void responseReceived();
    void onSendImage(const QPixmap *image,const QString name);
    void connectToServer(const QHostAddress serverAddr);
};

