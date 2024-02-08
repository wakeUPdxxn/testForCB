#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->send->setDisabled(true);
    ir.setAllocationLimit(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectFile_released()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    " ",
                                                    tr("Images (*.png *.xpm *.jpg *.svg *jpeg *ico)"));
    QFileInfo fi(path);
    imageName = fi.fileName();
    if(fi.size()>qint64(1e+9)){
        QMessageBox::information(this,"Attention","Selected file is to large");
    }
    image=new QImage;
    if(!image->load(path)){
        QMessageBox::critical(this,"Error","Error occured while loading image");
        delete image;
        image=NULL;
    }
}


void MainWindow::on_connectToServer_released()
{
    serverAddr=QHostAddress(ui->addrInput->text());
    if(!ui->addrInput->text().isEmpty()){
       ui->addrInput->setDisabled(true);
       ui->connectToServer->setDisabled(true);
       ui->selectFile->setDisabled(true);
       emit readyForConnection(serverAddr);
    }
}


void MainWindow::on_send_released()
{
    if(image==nullptr){
        QMessageBox::warning(this,"Attention","Image not selected");
        return;
    }
    emit needToSendImage(image,imageName);
}


void MainWindow::on_addrInput_editingFinished()
{
    serverAddr.setAddress(ui->addrInput->text().toInt());
}

void MainWindow::disableBlock()
{
    ui->addrInput->setDisabled(false);
    ui->connectToServer->setDisabled(false);
    ui->selectFile->setDisabled(false);
    ui->send->setDisabled(false);
}

