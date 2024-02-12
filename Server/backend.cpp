#include "backend.h"

Backend::Backend(QObject *parent)
    : QTcpServer{parent}
{
    makeSetUp();
    QImageReader::setAllocationLimit(500);
}

Backend::~Backend(){
    if(imageQueueProcessing.isRunning()){
        imageQueueProcessing.waitForFinished();
    }
    if(dbThread->isRunning()){
        dbThread->quit();
        dbThread->wait();
    }
    if(fmThread->isRunning()){
        fmThread->quit();
        fmThread->wait();
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
        m_dataManager = new LocalDataManager();

        fmThread=new QThread;
        m_dataManager->moveToThread(fmThread);

        connect(m_dataManager,&QThread::destroyed,m_dataManager,&LocalDataManager::deleteLater);
        connect(this,&Backend::newImage,m_dataManager,&LocalDataManager::onNewImage);
        connect(m_dataManager,&LocalDataManager::fmImageProcessed,this,&Backend::setFMimageProcessed);


        m_mainWindow=new MainWindow();

        connect(m_mainWindow,&MainWindow::getDBdata,this,&Backend::getDBdata);
        connect(this,&Backend::newImage,m_mainWindow,&MainWindow::onNewImage);
        connect(m_mainWindow,&MainWindow::destroyed,this,&Backend::close);

        m_mainWindow->ui->totalReceived->setText(QString::number(m_dataManager->GetImagesCount()));
        m_mainWindow->show();

        this->listen(QHostAddress::AnyIPv4,quint16(2323));
        connect(this,&Backend::newConnection,this,&Backend::connectionHandler);

        m_dbHandler = new DBhandler();  //initial db
        dbThread = new QThread();

        m_dbHandler->moveToThread(dbThread);
        connect(dbThread,&QThread::started,m_dbHandler,&DBhandler::open);
        connect(dbThread,&QThread::destroyed,m_dbHandler,&DBhandler::deleteLater);

        connect(this,&Backend::WriteToDb,m_dbHandler,&DBhandler::write);
        connect(this,&Backend::ReadFromDb,m_dbHandler,&DBhandler::read);

        connect(m_dbHandler,&DBhandler::readingFinished,m_mainWindow,&MainWindow::setTable);

        dbThread->start();
        fmThread->start();
    }
    catch(const QException &e){
        qDebug() << e.what();
        this->deleteLater();
    }
}

void Backend::waitForImageProcesed()
{
    while(!imageProcessingQueue.isEmpty()){
        if(isFMimageProcessed.load(std::memory_order_acquire)){
            auto image = imageProcessingQueue.back();
            delete image;

            imageProcessingQueue.pop_back();
            qDebug() << "one processed" << "last" << imageProcessingQueue.size();
            isFMimageProcessed.store(false,std::memory_order_release);
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
            if(messageSize==0){
                if(senderSock->bytesAvailable()<4){
                    break;
                }
                in>>messageSize;
            }
            if(senderSock->bytesAvailable()<messageSize){
                break;
            }
            messageSize=0;

            QPixmap *image=new QPixmap;
            QString name;
            in >> name;
            in >> *image;

            imageProcessingQueue.push_back(image);

            if(!imageQueueProcessing.isRunning()){
                imageQueueProcessing=QtConcurrent::run(std::bind(&Backend::waitForImageProcesed,this), 0);
            }
            emit WriteToDb(name,m_dataManager->getStoragePath()+name,QDate::currentDate().toString());
            emit newImage(image,name);

            senderSock->write(QByteArray("sent"));
        }
    }
}

void Backend::getDBdata()
{
    emit ReadFromDb(); //�� ����� ������.����� ������� ������ �� �������� ���� ����� � readfromdb � �� � getdbdata
}

void Backend::setFMimageProcessed()
{
    isFMimageProcessed.store(true,std::memory_order_release);
}

