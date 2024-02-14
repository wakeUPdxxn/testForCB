#include "client.h"

Client::Client(QObject *parent)
    : QObject{parent}
{
    m_socket = new QTcpSocket(this);

    connect(m_socket,&QTcpSocket::readyRead,this,&Client::responseReceived);
    connect(m_socket,&QTcpSocket::disconnected,m_socket,&QTcpSocket::deleteLater);

    this->setConnectionData();
}

Client::~Client()
{
    if(m_socket!=nullptr){
        m_socket->deleteLater();
    }
}

void Client::makeSetUp()
{
    if(autoConnect){
        connectToServer();
    }
}

void Client::responseReceived()
{
    QString response=m_socket->readLine();
    if(response=="received"){ //добавить колбек для получения от сервера оставшихся байт до загрузки и реализовать с их помощью процеccbyu
       emit imageSent();
    }
}

void Client::onSendImage(const QPixmap *image,const QString name)
{
    if(m_socket->state()!=QAbstractSocket::ConnectedState){
        emit setMessage("Error","Connection lost, try reconnect","warning");
    }

    data.clear();
    QDataStream out(&data,QIODevice::WriteOnly);

    out<<qint64(0)<<name<<*image;
    out.device()->seek(0);
    out << qint64(data.size()-sizeof(qint64));

    if(!m_socket->write(data)){
        emit setMessage("Error","Unable send message"+name,"warning");
    }

}

void Client::connectToServer()
{
    if(serverAddr.isNull()){
        emit setMessage("Connection settings","Ip is null. Please specify it in connection setting menu ","information");
        return;
    }
    if(!ipRex.match(serverAddr.toString()).hasMatch()){
        emit setMessage("Connection settings","Incorrect ip","information");
        return;
    }
    else if(port==0){
        emit setMessage("Connection settings","Incorrect port","information");
        return;
    }
    if(m_socket->state()==QAbstractSocket::ConnectedState){
        m_socket->waitForDisconnected(3000);
    }

    m_socket->connectToHost(serverAddr,port);//2323

    if(m_socket->waitForConnected(int(3000))){
        emit setMessage("Success","Connected","information");
    }
    else {
        emit setMessage("Error","Connection timeout reached","warning");
    }
}

void Client::setConnectionData()
{
    std::tie(serverAddr,port,autoConnect)=dm.getConnectionData();
}



