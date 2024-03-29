#include "client.h"
#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client *client=new Client;
    QThread *clientThread = new QThread;
    client->moveToThread(clientThread);
    QObject::connect(clientThread,&QThread::started,client,&Client::makeSetUp);
    QObject::connect(clientThread,&QThread::finished,client,&Client::deleteLater);

    MainWindow mainWindow;

    QObject::connect(&mainWindow,&MainWindow::quiting,clientThread,&QThread::quit);
    QObject::connect(&mainWindow,&MainWindow::readyForConnection,client,&Client::connectToServer);
    QObject::connect(&mainWindow,&MainWindow::SendImage,client,&Client::onSendImage);
    QObject::connect(&mainWindow,&MainWindow::onDataSaved,client,&Client::setConnectionData);

    QObject::connect(client,&Client::imageSent,&mainWindow,&MainWindow::onImageSent);
    QObject::connect(client,&Client::setMessage,&mainWindow,&MainWindow::showMessage);
    QObject::connect(client,&Client::changeStatus,&mainWindow,&MainWindow::onChangeStatus);

    clientThread->start();
    mainWindow.show();
    return a.exec();
}
