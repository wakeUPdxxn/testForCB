#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectionsettings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    QStringList pathes = QFileDialog::getOpenFileNames(this, tr("Open File"),
                                                    " ",
                                                    tr("Images (*.png *.xpm *.jpg *.svg *jpeg *ico)"));
    if(pathes.isEmpty()) {
        return;
    }
    foreach (auto path, pathes) {
        QFileInfo fi(path);
        QString imageName = fi.fileName();
        qDebug() << imageName ;
        if(fi.size()>qint64(1e+9)){
            QMessageBox::information(this,"Attention","Selected file is to large");
        }
        QPixmap *image = new QPixmap;
        if(!image->load(path)){
            QMessageBox::critical(this,"Error","Error occured while loading image");
            delete image;
            return;
        }
        ProccesingImage *imageToProcess = new ProccesingImage(image,imageName);
        imageProcesingQueue.push_back(imageToProcess);
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
    if(imageProcesingQueue.isEmpty()){
        QMessageBox::warning(this,"Attention","Image(s) not selected");
        return;
    }
    ui->send->setDisabled(true);
    emit SendImage(imageProcesingQueue.back()->p_Image,imageProcesingQueue.back()->name);
}


void MainWindow::on_addrInput_editingFinished()
{
    serverAddr.setAddress(ui->addrInput->text().toInt());
}

void MainWindow::disableBlock()
{
    if(!ui->addrInput->isEnabled()){
        ui->addrInput->setDisabled(false);
    }
    if(!ui->connectToServer->isEnabled()){
        ui->connectToServer->setDisabled(false);
    }
    if(!ui->selectFile->isEnabled()){
        ui->selectFile->setDisabled(false);
    }
    if(!ui->send->isEnabled()){
        ui->send->setDisabled(false);
    }
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
    connectionSettings->show();
}

void MainWindow::onRemoveImage()
{
    imageProcesingQueue.back()->deleteLater();
    imageProcesingQueue.pop_back();
    qDebug() << imageProcesingQueue.size();
}

