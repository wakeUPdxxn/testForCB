#include "dbhandler.h"
#include "qthread.h"
#include <QScopedPointer>

DBhandler::DBhandler(QObject *parent)
    : QObject{parent}
{

}

DBhandler::~DBhandler(){
    qDebug() << "db handler destroyed";
}

void DBhandler::open()
{
    db = QSqlDatabase::addDatabase(dbDriveName);
    db.setDatabaseName(dbName);
    db.setHostName(hostName);
    db.open();
}

void DBhandler::write(const QString imageName, const QString path, const QString date)
{
    QSqlQuery query;
    query.prepare("INSERT INTO images(name, path, date) VALUES(?, ?, ?)");
    query.addBindValue(imageName);
    query.addBindValue(path);
    query.addBindValue(date);

    if(query.exec()){
        qDebug() << "writed";
    }
    else{
        qDebug() << query.lastError();
    }
}

void DBhandler::read()
{
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel;
    query.prepare("SELECT * FROM images");
    query.exec();
    model->setQuery(std::move(query));
    emit readingFinished(model);
}

