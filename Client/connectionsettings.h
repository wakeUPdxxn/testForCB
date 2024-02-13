#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

class ConnectionSettings : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionSettings(QWidget *parent = nullptr);

private:
    QLabel address_lbl;
    QLabel port_lbl;
    QLineEdit addrInput;
    QLineEdit portInput;
    QPushButton save;
    QString address;
    QString port;
    QGridLayout layout;
    QCheckBox isAutoConnect;

signals:
    void dataSaved();

private slots:
    void onSavePressed();
};

