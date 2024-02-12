#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QImageReader>
#include <QtConcurrent>
#include <QScreen>
#include "connectionsettings.h"

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

private slots:
    void on_selectFile_released();
    void on_connectToServer_released();
    void on_send_released();
    void on_addrInput_editingFinished();

private:
    QHostAddress serverAddr;
    QScreen *p_Screen=nullptr;

    struct ProccesingImage{
        explicit ProccesingImage(QPixmap*img =nullptr,const QString imageName="")
            :p_Image(img),name(imageName) {}
            ~ProccesingImage(){
                delete p_Image;
                p_Image = nullptr;
            }
            void deleteLater(){
                this->~ProccesingImage();
            }
        QPixmap *p_Image;
        QString name;
    };
    QQueue<ProccesingImage*>imageProcesingQueue;
    QStringList imagesToSend;

public slots:
    void disableBlock();
    void showMessage(const QString &title,const QString &text,const QString &type);
    void onConnectionSettingsClicked(QAction* action);
    void onImageSent();

signals:
    void readyForConnection(const QHostAddress serverAddr);
    bool SendImage(const QPixmap *image,const QString name);
    void quiting();
};


