#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbsettings.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QNetworkInterface>
#include <localdatamanager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("Server GUI");
    QScreen* p_Screen = this->screen();
    this->setGeometry(QRect(p_Screen->geometry().left()+p_Screen->geometry().width()/4,
                            p_Screen->geometry().top()+p_Screen->geometry().height()/4,
                            p_Screen->geometry().width()/3,
                            p_Screen->geometry().height()/2));

    m_popUp = new PopUp;

    connect(ui->Menu,SIGNAL(triggered(QAction*)),SLOT(onDbSettingsClicked(QAction*)));
}

MainWindow::~MainWindow()
{
    m_popUp->deleteLater();
    delete ui;
}

void MainWindow::setTable(QSqlQueryModel *model)
{
    QTableView *table = new QTableView();
    table->setWindowFlag(Qt::WindowStaysOnTopHint);
    table->setWindowTitle("Database data");
    table->setAttribute(Qt::WA_DeleteOnClose);
    table->setModel(model);

    QSize size(this->width()/2,this->height()/1.5);
    table->setMinimumSize(size);
    table->setStyleSheet("QHeaderView::section { background-color:rgb(126, 132, 255); }"
                         "QTableView {"
                            "font: bold 20px;"
                            "color: rgb(252, 238, 255);"
                            "background-color: #3c3b47;"
                            "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0.5, y2: 0.5,"
                            "stop: 0 blue, stop: 1 rgb(83, 189, 255));"
                         "}"
                         "font: bold 20px;");
    table->show();
}

void MainWindow::onNewImage(const QPixmap *image, const QString name)
{
    m_popUp->setText("New image! " + name);
    m_popUp->show();

    QSize messageWigdetSize(ui->frame->frameRect().height()/2,ui->frame->frameRect().width()/4);

    QListWidgetItem *message = new QListWidgetItem;
    message->setSizeHint(QSize(messageWigdetSize));
    ui->listWidget->addItem(message);

    QLabel *message_img_lbl = new QLabel("");
    message_img_lbl->setPixmap(image->scaled(messageWigdetSize));

    QLabel *message_imgName_lbl = new QLabel(name);

    QWidget *msg = new QWidget;
    msg->setLayout(new QVBoxLayout);
    msg->layout()->addWidget(message_imgName_lbl);
    msg->layout()->addWidget(message_img_lbl);

    ui->listWidget->setItemWidget(message,msg);
    ui->totalReceived->setText(QString::number(ui->totalReceived->text().toInt()+1));
}


void MainWindow::on_getDB_pb_released()
{
   emit getDBdata();
}

void MainWindow::onDbSettingsClicked(QAction *action)
{

    if(action->text()=="Database settings"){
        DBsettings *dbSettings=new DBsettings();

        connect(dbSettings,&DBsettings::dataSaved,this,&MainWindow::onDbDataSaved);
        connect(dbSettings,&DBsettings::setMessage,this,&MainWindow::onSetMessage);
        connect(dbSettings,&DBsettings::dataSaved,dbSettings,&DBsettings::deleteLater);

        dbSettings->setGeometry(QRect(QCursor::pos().x(),QCursor::pos().y(),this->size().width()/2,this->size().height()));
        dbSettings->setFixedSize(dbSettings->geometry().width()+160,dbSettings->geometry().height()-200);
        dbSettings->show();
    }
    else{
        QList<QHostAddress>allowedAddresses{QNetworkInterface::allAddresses()};
        allowedAddresses.removeIf([](const QHostAddress &address){
            return (address.toString().contains(':')) ?true :false;
        });

        QString infoStr="Server listening at interfaces: \n";

        foreach (auto address, allowedAddresses) {
            infoStr+=address.toString()+"\n";
        }
        infoStr+="on port:" + QString::number(std::get<1>(LocalDataManager::getConfigData())) + "\n"
                +"and listening " + std::get<0>(LocalDataManager::getConfigData()) + " address(es)";

        QMessageBox::information(this,"Server info",infoStr);
    }
}

void MainWindow::onSetMessage(const QString &title,const QString &text,const QString &type)
{
    if(type=="information"){
        QMessageBox::information(this,title,text);
    }
    else if(type=="warning"){
        QMessageBox::warning(this,title,text);
    }
    else {
        QMessageBox::critical(this,title,text);
    }
}


