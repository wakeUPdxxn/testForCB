#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->send->setDisabled(true);
    setWindowTitle("Client");

    QImageReader::setAllocationLimit(500);

    p_Screen=this->screen();

    connect(ui->Menu,SIGNAL(triggered(QAction*)),SLOT(onConnectionSettingsClicked(QAction*)));
}

MainWindow::~MainWindow()
{
    emit quiting();
    delete ui;
}

void MainWindow::on_selectFile_released()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    " ",
                                                    tr("Images (*.png *.xpm *.jpg *.svg *jpeg *ico)"));
    if(path.isEmpty()) {
        return;
    }
    QFileInfo fi(path);
    imageName = fi.fileName();

    if(fi.size()>qint64(1e+9)){
        QMessageBox::information(this,"Attention","Selected file is to large");
    }
    image=new QPixmap;
    if(!image->load(path)){
        QMessageBox::critical(this,"Error","Error occured while loading image");
        delete image;
        image=nullptr;
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

void MainWindow::showMessage(const QString &title,const QString &text,const QString &type)
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

void MainWindow::onConnectionSettingsClicked(QAction *action)
{
    ConnectionSettings *connectionSettings=new ConnectionSettings;
    connectionSettings->setGeometry(QRect(QCursor::pos().x(),QCursor::pos().y(),this->size().width()/3,this->size().height()/2));
}

