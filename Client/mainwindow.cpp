#include <QImageReader>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectionsettings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->setWordWrap(true);
    setWindowTitle("Client");

    QImageReader::setAllocationLimit(500);

    p_Screen=this->screen();

    connect(ui->Menu,SIGNAL(triggered(QAction*)),SLOT(onConnectionSettingsClicked(QAction*)));
}

MainWindow::~MainWindow()
{
    if(!imageProcesingQueue.isEmpty()){
        while(!imageProcesingQueue.isEmpty()){
            auto img = imageProcesingQueue.back();
            img->deleteLater();
            imageProcesingQueue.pop_back();
        }
    }
    emit quiting();
    delete ui;
}

void MainWindow::on_selectFile_released()
{
    ui->selectFile->setGraphicsEffect(nullptr);

    imagesToSend = QFileDialog::getOpenFileNames(this, tr("Open File"),
                                                    " ",
                                                    tr("Images (*.png *.xpm *.jpg *.svg *jpeg *ico)"));
    if(imagesToSend.isEmpty()) {
        return;
    }
    foreach (auto path, imagesToSend) {
        QFileInfo fi(path);
        qint64 size = fi.size();
        QString imageName = fi.fileName();
        if(size>qint64(1e+9)){
            QMessageBox::information(this,"Attention","Selected file is to large");
        }

        QPixmap *image = new QPixmap;
        if(!image->load(path)){
            QMessageBox::critical(this,"Error","Error occured while loading image");
            delete image;
            return;
        }

        ui->listWidget->addItem(imageName);

        ProccesingImage *imageToProcess = new ProccesingImage(image,imageName);
        imageProcesingQueue.push_back(imageToProcess);
    }
}

void MainWindow::on_selectFile_pressed() {
    setShadow((QWidget*)sender());
}


void MainWindow::on_connectToServer_released()
{
    ui->connectToServer->setGraphicsEffect(nullptr);
    ui->connectToServer->setDisabled(true);
    ui->selectFile->setDisabled(true);
    emit readyForConnection();
}

void MainWindow::on_connectToServer_pressed()
{
    setShadow((QWidget*)sender());
}

void MainWindow::on_send_pressed(){
    setShadow((QWidget*)sender());
}

void MainWindow::on_send_released()
{
    ui->send->setGraphicsEffect(nullptr);
    if(imagesToSend.isEmpty()){
        QMessageBox::warning(this,"Attention","Image(s) not selected");
        return;
    }

    imagesToSend.clear();
    ui->listWidget->clear();
    ui->sending_lbl->setText("Sending 1 of " + QString::number(imageProcesingQueue.size()));
    ui->progressBar->setValue(0);

    emit SendImage(imageProcesingQueue.back()->p_Image,imageProcesingQueue.back()->name);
}

void MainWindow::setShadow(QWidget *widget)
{
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(3);
    widget->setGraphicsEffect(effect);
}

void MainWindow::disableBlock()
{
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
    Q_UNUSED(action)

    ConnectionSettings *connectionSettings=new ConnectionSettings;
    connect(connectionSettings,&ConnectionSettings::dataSaved,this,&MainWindow::onDataSaved);
    connectionSettings->setGeometry(QRect(QCursor::pos().x(),QCursor::pos().y(),this->size().width()/3,this->size().height()/2));
    connectionSettings->setFixedSize(connectionSettings->geometry().width()+160,connectionSettings->geometry().height());
    connectionSettings->show();
}

void MainWindow::onImageSent()
{
    static qint64 doneCount{1};
    static qint64 allCount = imageProcesingQueue.size()+2;
    imageProcesingQueue.back()->deleteLater();
    imageProcesingQueue.pop_back();

    if(!imageProcesingQueue.isEmpty()){
        onChangeStatus(doneCount*100/allCount);
        doneCount++;
        ui->sending_lbl->setText("Sending" +QString::number(doneCount) + "of " + QString::number(allCount));
        emit SendImage(imageProcesingQueue.back()->p_Image,imageProcesingQueue.back()->name);
    }
    else{
        ui->sending_lbl->setText("Done!");
        onChangeStatus(100);
        ui->listWidget->clear();
    }
}

void MainWindow::onChangeStatus(const int percent)
{
    this->ui->progressBar->setValue(percent);
}



