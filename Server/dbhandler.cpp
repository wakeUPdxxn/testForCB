#include "dbhandler.h"
#include <localdatamanager.h>

DBhandler::DBhandler(QObject *parent)
    : QObject{parent}
{
    dbData=LocalDataManager::getDbData();
}

DBhandler::~DBhandler(){
    if(db.open()){
        db.close();
    }
}

void DBhandler::Reopen(){ //Для того, чтобы база открылась с обновленными данными из dbSettings окна
    if(db.isOpen()){
        db.close();
        dbData=LocalDataManager::getDbData();
        this->open();
    }
    else{
        dbData=LocalDataManager::getDbData();
        this->open();
    }
}

void DBhandler::open()
{
    if(!QFile::exists(dbData["dbName"].toString())){
        emit setMessage("Database error","Database doesnt not exists","error");
        return;
    }
    db = QSqlDatabase::addDatabase(dbData["dbDriverName"].toString());
    db.setDatabaseName(dbData["dbName"].toString());
    db.setHostName(dbData["hostName"].toString());
    db.setUserName(dbData["user"].toString());
    db.setPassword(dbData["password"].toString());
    db.open();
    if(db.isOpen()){
        emit setMessage("Success","Database connected","information");
    }
    else{
        emit setMessage("Connection faild","Unable connect to database.Please,check settings","information");
    }
}

void DBhandler::write(const QString imageName, const QString path, const QString date)
{
    QSqlQuery query;
    query.prepare("INSERT INTO images(name, path, date) VALUES(?, ?, ?)");
    query.addBindValue(imageName);
    query.addBindValue(path);
    query.addBindValue(date);

    if(!query.exec()){
        emit setMessage("Query error",query.lastError().text(),"critical");
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



