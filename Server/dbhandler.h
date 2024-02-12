#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>

class DBhandler : public QObject
{
    Q_OBJECT
public:
    explicit DBhandler(QObject *parent = nullptr);
    ~DBhandler();
    void logIn(const QString login,const QString password);
    void open();
    void close();

public slots:
    void write(const QString imageName,const QString path,const QString date);
    void read();

signals:
    void readingFinished(QSqlQueryModel* model);

private:
    QSqlDatabase db;
    quint16 port;
    QString dbName="..\\testDb.db";
    QString hostName="localhost";
    QString userName;
    QString password;
    QString dbDriveName="QSQLITE";
    QString dbPath;
};

