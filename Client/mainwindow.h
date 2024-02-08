#include <QMainWindow>
#include <QFileDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QImageReader>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectFile_released();
    void on_connectToServer_released();
    void on_send_released();
    void on_addrInput_editingFinished();

private:
    Ui::MainWindow *ui;
    QImage *image=nullptr;
    QString imageName;
    QHostAddress serverAddr;
    QImageReader ir;

public slots:
    void disableBlock();

signals:
    void readyForConnection(const QHostAddress serverAddr);
    void needToSendImage(const QImage *image,const QString name);
};


