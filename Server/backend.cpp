#include "backend.h"

Backend::Backend(QObject *parent)
    : QTcpServer{parent}
{
    makeSetUp();
    imr.setAllocationLimit(500);
}

Backend::~Backend(){
    if(imageQueueProccessing.isRunning()){
        imageQueueProccessing.waitForFinished();
    }
    if(dbThread->isRunning()){
        dbThread->quit();
        dbThread->wait();
    }
    if(m_mainWindow!=nullptr){
        m_mainWindow->deleteLater();
    }
    foreach (auto socket, clients) {
        socket->deleteLater();
    }
}
void Backend::makeSetUp()
{
    try{
        m_mainWindow=new MainWindow();
        m_mainWindow->show();
        connect(m_mainWindow,&MainWindow::getDBdata,this,&Backend::getDBdata);
        connect(this,&Backend::newImage,m_mainWindow,&MainWindow::onNewImage);
        connect(m_mainWindow,&MainWindow::ImageProccessed,this,&Backend::setMVimageProccesed);
        connect(m_mainWindow,&MainWindow::destroyed,this,&Backend::close); //убрать после переопределения слоуз ивента в mainwindow

        this->listen(QHostAddress::AnyIPv4,quint16(2323));
        connect(this,&Backend::newConnection,this,&Backend::connectionHandler);

        m_dataManager = new LocalDataManager(this);
        connect(m_dataManager,&LocalDataManager::ImageProccessed,this,&Backend::setFMimageProccesed);

        m_dbHandler = new DBhandler();  //initial db
        dbThread = new QThread();

        m_dbHandler->moveToThread(dbThread);
        connect(dbThread,&QThread::started,m_dbHandler,&DBhandler::open);
        connect(dbThread,&QThread::destroyed,m_dbHandler,&DBhandler::deleteLater);

        connect(this,&Backend::WriteToDb,m_dbHandler,&DBhandler::write);
        connect(this,&Backend::ReadFromDb,m_dbHandler,&DBhandler::read);

        connect(m_dbHandler,&DBhandler::readingFinished,m_mainWindow,&MainWindow::setTable);
        dbThread->start();
    }
    catch(const QException &e){
        qDebug() << e.what();
        this->deleteLater();
    }
}

void Backend::waitForImageProccesed()
{
    while(!imageProccesingQueue.isEmpty()){
        if(isFMimageProccesed.load(std::memory_order_acquire) && isMWimageProccesed.load(std::memory_order_acquire)){
            auto image = imageProccesingQueue.back();
            delete image;
            image=nullptr;

            imageProccesingQueue.pop_back();

            isMWimageProccesed.store(false,std::memory_order_release);
            isFMimageProccesed.store(false,std::memory_order_release);
        }
    }
}

void Backend::connectionHandler()
{
    m_clientSock = this->nextPendingConnection();

    connect(m_clientSock,&QTcpSocket::readyRead,this,&Backend::onServerReadyRead);
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

void Backend::onServerReadyRead()
{
    QTcpSocket *senderSock=(QTcpSocket*)sender();
    QDataStream in(senderSock);
    if(in.status()==QDataStream::Ok){
        for( ; ; ){
            if(nextBlockSize==0){
                if(senderSock->bytesAvailable()<4){
                    break;
                }
                in>>nextBlockSize;
            }
            if(senderSock->bytesAvailable()<nextBlockSize){
                break;
            }
            qDebug() << "received";
            nextBlockSize=0;
            QPixmap *image=new QPixmap;
            QString name;
            in >> name;
            in >> *image;

            imageProccesingQueue.push_back(image);

            imageQueueProccessing=QtConcurrent::run(std::bind(&Backend::waitForImageProccesed,this), 0);

            std::ignore = QtConcurrent::run(std::bind(&LocalDataManager::saveImage,m_dataManager,image,name), 0);
            emit WriteToDb(name,m_dataManager->getRootPath()+name,QDate::currentDate().toString());
            emit newImage(image,name);
        }
    }
}

void Backend::getDBdata()
{
    emit ReadFromDb();
}

void Backend::setFMimageProccesed()
{
    isMWimageProccesed.store(true,std::memory_order_release);
}

void Backend::setMVimageProccesed()
{
    isFMimageProccesed.store(true,std::memory_order_release);
}
