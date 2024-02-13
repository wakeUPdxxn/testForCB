#include "connectionsettings.h"
#include "localdatamanager.h"

ConnectionSettings::ConnectionSettings(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Connection Settings");
    this->setStyleSheet("QWidget { background-color: #4f4f4f;"
                        "font: bold 20px solid;"
                        "color: #222222 }"

                        "QPushButton { "
                        "background-color: rgb(255, 73, 56);"
                        "border: 2px solid rgb(255, 73, 56);"
                        "border-radius: 8px }"

                        "QLineEdit { "
                        "border: 2px solid gray;"
                        "border-radius: 8px };"
                        );

    address_lbl.setText("Server address: ");
    address_lbl.setAlignment(Qt::AlignHCenter| Qt::AlignRight);

    port_lbl.setText("Server port: ");
    address_lbl.setAlignment(Qt::AlignHCenter | Qt::AlignRight);

    save.setText("Save");

    isAutoConnect.setText("Auto connect");

    layout.addWidget(&address_lbl, 0, 0);
    layout.addWidget(&addrInput, 0, 1);
    layout.addWidget(&port_lbl, 1, 0);
    layout.addWidget(&portInput, 1, 1);
    layout.addWidget(&save, 2, 0);
    layout.addWidget(&isAutoConnect, 2, 1);

    setLayout(&layout);

    connect(&save,&QPushButton::clicked,this,&ConnectionSettings::onSavePressed);
}

void ConnectionSettings::onSavePressed()
{
    if(this->addrInput.text()=="localhost" || this->addrInput.text()=="LOCALHOST"){
        LocalDataManager::setConnectionData(QHostAddress::LocalHost,quint16(this->portInput.text().toUInt()),isAutoConnect.isChecked());
    }
    else{
        LocalDataManager::setConnectionData(QHostAddress(this->addrInput.text()),quint16(this->portInput.text().toUInt()),isAutoConnect.isChecked());
    }
    emit dataSaved();
}



