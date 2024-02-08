#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QScreen *screen=::MainWindow::screen();
    this->setGeometry(QRect(screen->geometry().left()+screen->geometry().width()/4,
                            screen->geometry().top()+screen->geometry().height()/4,
                            screen->geometry().width()/3,
                            screen->geometry().height()/2));
    model=new QStandardItemModel;
    ui->listView->setModel(model);
    m_popUp = new PopUp;
    m_popUp->setText("New image!");
}

MainWindow::~MainWindow()
{
    m_popUp->deleteLater();
    model->deleteLater();
    delete ui;
}

void MainWindow::setTable(QSqlQueryModel *model)
{
    QTableView *table = new QTableView();
    table->setWindowFlag(Qt::WindowStaysOnTopHint);
    table->setBaseSize(QSize(250,250));
    table->setWindowTitle("DBdata");
    table->setModel(model);
    table->hideColumn(0);
    table->show();
}

void MainWindow::onNewImage(QImage &image, const QString name)
{
    m_popUp->show();
    QStandardItem *text = new QStandardItem;
    text->setText(name);
    QStandardItem *item = new QStandardItem; //
    item->setData(QVariant(QPixmap::fromImage(image)), Qt::DecorationRole);
    model->appendRow(text);
    model->appendRow(item);
    ui->listView->update();
}


void MainWindow::on_getDB_pb_released()
{
   emit getDBdata();
}



