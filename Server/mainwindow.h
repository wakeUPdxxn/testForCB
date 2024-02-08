#pragma once

#include "qsqlquerymodel.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QTableView>
#include <QScreen>
#include <QStandardItemModel>
#include <popup.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;

signals:
    void getDBdata();

public slots:
    void setTable(QSqlQueryModel* model);
    void onNewImage(QImage &image,const QString name);

private slots:
    void on_getDB_pb_released();

private:
    QStandardItemModel *model;
    PopUp *m_popUp;
};
