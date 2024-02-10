#include "client.h"

Client::Client(QObject *parent)
    : QObject{parent}
{
    m_socket = new QTcpSocket(this);

    connect(m_socket,&QTcpSocket::readyRead,this,&Client::responseReceived);
    connect(m_socket,&QTcpSocket::disconnected,m_socket,&QTcpSocket::deleteLater);
}

Client::~Client()
{
    if(m_socket!=nullptr){
        m_socket->deleteLater();
    }
}

void Client::responseReceived() //вероятно удалить
{
    QString response;
    QDataStream in(m_socket);
    in >> response;
}

void Client::sendImage(const QPixmap *image,const QString name)
{
    if(m_socket->state()!=QAbstractSocket::ConnectedState){
        emit setMessage("Error","Connection lost, try reconnect","warning");
        return;
    }

    data.clear();
    QDataStream out(&data,QIODevice::WriteOnly);

    out<<qint64(0)<<name<<*image;
    out.device()->seek(0);
    out << qint64(data.size()-sizeof(qint64));

    if(m_socket->write(data)==-1){
        emit setMessage("Error","Unable send message"+name,"warning");
    }

    delete image;
    image = nullptr;
}

void Client::connectToServer(const QHostAddress serverAddr)
{
    if(m_socket->state()==QAbstractSocket::ConnectedState){
        m_socket->waitForDisconnected(3000);
    }

    m_socket->connectToHost(serverAddr,2323);//2323

    if(m_socket->waitForConnected(int(3000))){
        emit disableUiBlock();
        emit setMessage("Success","Connected","information");
    }
    else {
        emit disableUiBlock();
        emit setMessage("Error","Connection timeout reached","warning");
    }
}



