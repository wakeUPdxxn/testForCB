#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);//remove after overriging of closeEvent

    QScreen* p_Screen = this->screen();
    this->setGeometry(QRect(p_Screen->geometry().left()+p_Screen->geometry().width()/4,
                            p_Screen->geometry().top()+p_Screen->geometry().height()/4,
                            p_Screen->geometry().width()/3,
                            p_Screen->geometry().height()/2));

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
    table->setAttribute(Qt::WA_DeleteOnClose);
    table->setBaseSize(QSize(250,250));
    table->setWindowTitle("DBdata");
    table->setModel(model);
    table->hideColumn(0);
    table->show();
}

void MainWindow::onNewImage(const QPixmap *image, const QString name)
{
    m_popUp->show();
    QStandardItem *text = new QStandardItem;
    text->setText(name);
    QStandardItem *item = new QStandardItem;
    item->setData(*image, Qt::DecorationRole);
    model->appendRow(text);
    model->appendRow(item);
    ui->listView->update();
    emit ImageProccessed();
}


void MainWindow::on_getDB_pb_released()
{
   emit getDBdata();
}



