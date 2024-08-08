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
    void Reopen();

signals:
    void readingFinished(QSqlQueryModel* model);
    void setMessage(const QString &title,const QString &text,const QString &type);

private:
    QSqlDatabase db;
    QVariantMap dbData;
    void makeSetUp();
    void CheckTableExists();
};

