#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>

#include <QWidget>

class DBsettings : public QWidget
{
    Q_OBJECT
public:
    explicit DBsettings(QWidget *parent = nullptr);

private:
    QLabel name_lbl;
    QLabel host_lbl;
    QLabel user_lbl;
    QLabel password_lbl;
    QLabel driver_lbl;

    QLineEdit nameInput;
    QLineEdit hostInput;
    QLineEdit userInput;
    QLineEdit passwordInput;
    QLineEdit driverInput;

    QPushButton save;
    QGridLayout layout;

signals:
    void dataSaved();
    void setMessage(const QString &title,const QString &text,const QString &type);

private slots:
    void onSavePressed();
};

