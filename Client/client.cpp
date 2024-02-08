#include "client.h"

Client::Client(QObject *parent)
    : QObject{parent}
{
    m_mainWindow=new MainWindow();
    m_mainWindow->show();
    m_socket = new QTcpSocket(this);
    connect(m_mainWindow,&MainWindow::readyForConnection,this,&Client::connectToServer);
    connect(m_mainWindow,&MainWindow::needToSendImage,this,&Client::sendImage);
    connect(this,&Client::disableUiBlock,m_mainWindow,&MainWindow::disableBlock);
    connect(m_socket,&QTcpSocket::readyRead,this,&Client::responseReceived);
    connect(m_socket,&QTcpSocket::disconnected,m_socket,&QTcpSocket::deleteLater);
}

Client::~Client()
{
    if(m_socket!=nullptr){
        m_socket->deleteLater();
    }
    if(m_mainWindow!=nullptr){
        m_mainWindow->deleteLater();
    }
}

void Client::responseReceived()
{
    QString response;
    QDataStream in(m_socket);
    in >> response;
}

void Client::sendImage(const QImage *image,const QString name)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << name;
    out << *image;
    if(m_socket->write(data)==-1){
        qDebug() << QMessageBox::warning(m_mainWindow,"Error","Cannot send message"+name);
    }
    //как только отправится, чистим указатель на картинку
}

void Client::connectToServer(const QHostAddress serverAddr)
{
    m_socket->connectToHost(serverAddr,2323);
    if(m_socket->waitForConnected(int(10000))){
        emit disableUiBlock();
        QMessageBox::information(m_mainWindow,"Success","Connected");
    }
    else{
        emit disableUiBlock();
        QMessageBox::information(m_mainWindow,"Error","Connection timeout reached");
    }
}


