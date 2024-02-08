#include "backend.h"

Backend::Backend(QObject *parent)
    : QTcpServer{parent}
{
    makeSetUp();
    imr.setAllocationLimit(500);
}

Backend::~Backend(){
    if(m_mainWindow!=nullptr){
        m_mainWindow->deleteLater();
    }
}
void Backend::makeSetUp()
{
    try{
        m_mainWindow=new MainWindow();
        m_mainWindow->show();
        connect(m_mainWindow,&MainWindow::getDBdata,this,&Backend::getDBdata);
        connect(this,&Backend::newImage,m_mainWindow,&MainWindow::onNewImage);

        this->listen(QHostAddress::AnyIPv4,quint16(2323));
        connect(this,&Backend::newConnection,this,&Backend::connectionHandler);

        m_dataManager = new LocalDataManager(this); //initial fileManager унести //унести в тред и вместо qtconcurrent в конце файла,завязать на сигналы
        m_dbHandler = new DBhandler();          //initial db

        dbThread = new QThread(this);

        m_dbHandler->moveToThread(dbThread);
        connect(dbThread,&QThread::started,m_dbHandler,&DBhandler::open);
        connect(dbThread,&QThread::destroyed,m_dbHandler,&DBhandler::deleteLater);

        connect(this,&Backend::WriteToDb,m_dbHandler,&DBhandler::write);
        connect(m_dbHandler,&DBhandler::readingFinished,m_mainWindow,&MainWindow::setTable);
        connect(this,&Backend::ReadFromDb,m_dbHandler,&DBhandler::read);
        dbThread->start();
    }
    catch(const QException &e){
        qDebug() << e.what();
        this->deleteLater();
    }
}

void Backend::connectionHandler()
{
    m_clientSock = this->nextPendingConnection();
    qDebug() << "client connected";

    connect(m_clientSock,&QTcpSocket::readyRead,this,&Backend::imageReceived);
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

void Backend::imageReceived()
{
    QTcpSocket *senderSock=(QTcpSocket*)sender();
    QDataStream in(senderSock);
    QString name;
    QImage image;
    in >> name;
    in >> image;
    //m_mainWindow->ui->label->setPixmap(QPixmap::fromImage(image));//убрать и ui в приват в main window
    emit newImage(image,name);
    std::ignore = QtConcurrent::run(std::bind(&LocalDataManager::saveImage,m_dataManager,image,name), 0);
    emit WriteToDb(name,m_dataManager->getRootPath()+name,QDate::currentDate().toString());
}

void Backend::getDBdata()
{
    emit ReadFromDb();
}
