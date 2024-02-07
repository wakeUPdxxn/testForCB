#include "backend.h"

Backend::Backend(QObject *parent)
    : QTcpServer{parent}
{
    makeSetUp();
}

Backend::~Backend(){

}
void Backend::makeSetUp()
{
    try{
        this->listen(QHostAddress::AnyIPv4,quint16(2323));
        m_dataManager = new LocalDataManager(this); //initial fileManager
        m_dbHandler = new DBhandler(this);          //initial db
        connect(this,&Backend::newConnection,this,&Backend::connectionHandler);
    }
    catch(const QException &e){
        qDebug() << e.what();
    }
}

void Backend::connectionHandler()
{
    m_clientSock = this->nextPendingConnection();
    qDebug() << "client connected";
    QByteArray callBack;
    QDataStream data(&callBack,QIODevice::WriteOnly);
    data << QString("connected");
    m_clientSock->write(callBack);

    connect(m_clientSock,&QTcpSocket::readyRead,this,&Backend::messageReceived);
    connect(m_clientSock,&QTcpSocket::disconnected,this,&Backend::disconnectionEvent);

    clients.emplace(m_clientSock->socketDescriptor(),{m_clientSock, " "});

void Backend::disconnectionEvent()
{
    QTcpSocket *clientSender=(QTcpSocket*)sender();
}

void Backend::messageReceived()
{
    QTcpSocket *clientSender=(QTcpSocket*)sender();
}
