#include "connectionsettings.h"

ConnectionSettings::ConnectionSettings(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Connection Settings");
    this->setStyleSheet("QWidget { background-color: #2F4F4F;}"
                        "QLabel { color : black; }"

                        "QPushButton { "
                        "background-color: blue; "
                        "border: 2px solid;"
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
    //обращаемся к классу управления данными для записи в json файл
}



