#pragma once

#include <QMainWindow>
#include <QHostAddress>
#include <QtConcurrent>
#include <QScreen>

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
    void on_selectFile_pressed();
    void on_connectToServer_released();
    void on_connectToServer_pressed();
    void on_send_released();
    void on_send_pressed();

private:
    QScreen *p_Screen=nullptr;
    void setShadow(QWidget *obj);

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
        const QString name;
    };
    QQueue<ProccesingImage*>imageProcesingQueue;
    QStringList imagesToSend;

public slots:
    void showMessage(const QString &title,const QString &text,const QString &type);
    void onConnectionSettingsClicked(QAction* action);
    void onImageSent();
    void onChangeStatus(const int percent);

signals:
    void readyForConnection();
    bool SendImage(const QPixmap *image,const QString name);
    void quiting();
    void onDataSaved(); //cathing dataSaved signal from connection settings and rethrowing to client;
};


