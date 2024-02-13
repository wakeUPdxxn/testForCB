#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);//remove after overriging of closeEvent
    this->setWindowTitle("Server GUI");
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

    table->hideColumn(0);
    table->show();
}

void MainWindow::onNewImage(const QPixmap *image, const QString name)
{
    m_popUp->show();

    QStandardItem *text = new QStandardItem;
    text->setText(name);
    QStandardItem *item = new QStandardItem;
    item->setData(image->scaled(ui->frame->frameRect().height()/2,ui->frame->frameRect().width()/4), Qt::DecorationRole);

    model->appendRow(text);
    model->appendRow(item);
    ui->listView->update();
    ui->totalReceived->setText(QString::number(ui->totalReceived->text().toInt()+1));
}


void MainWindow::on_getDB_pb_released()
{
   emit getDBdata();
}



