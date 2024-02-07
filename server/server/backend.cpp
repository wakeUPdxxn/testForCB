#include "backend.h"
#include "qpixmap.h"

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

    clients.push_back(m_clientSock);
}

void Backend::disconnectionEvent()
{
    QTcpSocket *clientSender=(QTcpSocket*)sender();
    clients.removeOne(clientSender);
    if(clientSender!=nullptr){
        clientSender->deleteLater();
    }
}

void Backend::messageReceived()
{
    quint16 nextBlockSize{0};
    QTcpSocket *senderSock=(QTcpSocket*)sender();
    QDataStream in(senderSock);
    QString name;
    QPixmap image;
    if(in.status()==QDataStream::Ok){
        for( ; ; ){
            if(nextBlockSize==0){
                if(senderSock->bytesAvailable()<2){
                      break;
                }
                in >> nextBlockSize;
            }
            if(senderSock->bytesAvailable() < nextBlockSize){
                break;
            }
            in >> name;
            in >> image;
            nextBlockSize=0;
        }
    }
}
