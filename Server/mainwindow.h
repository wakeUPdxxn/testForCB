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
    void mwImageProcessed();
    void onDbDataSaved(); //for rethrowing signal "dataSaved" from dbsettings to backend

public slots:
    void onSetMessage(const QString &title,const QString &text,const QString &type);
    void setTable(QSqlQueryModel* model);
    void onNewImage(const QPixmap *image,const QString name);
    void onDbSettingsClicked(QAction* action);

private slots:
    void on_getDB_pb_released();

private:
    QStandardItemModel *model;
    PopUp *m_popUp;
};
