#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QException>
#include "dbhandler.h"
#include "localdatamanager.h"

class Backend : public QTcpServer
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

private:
    QHash<quint16,QPair<QTcpSocket*,QString>> clients;
    DBhandler *m_dbHandler;
    LocalDataManager *m_dataManager;
    QTcpSocket *m_clientSock;
private:
    void makeSetUp();

private slots:
    void connectionHandler();

public slots:
    void disconnectionEvent();
    void messageReceived();
};

