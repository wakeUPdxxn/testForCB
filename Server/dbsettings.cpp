#include "dbsettings.h"
#include "localdatamanager.h"

DBsettings::DBsettings(QWidget *parent)
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

    name_lbl.setText("Database name/path: ");
    name_lbl.setAlignment(Qt::AlignHCenter| Qt::AlignRight);

    host_lbl.setText("DB host(default: localhost): ");
    host_lbl.setAlignment(Qt::AlignHCenter | Qt::AlignRight);

    user_lbl.setText("User name(optional): ");
    user_lbl.setAlignment(Qt::AlignHCenter | Qt::AlignRight);

    password_lbl.setText("Password(optional): ");
    password_lbl.setAlignment(Qt::AlignHCenter | Qt::AlignRight);

    driver_lbl.setText("DB driver(default: QSQLITE): ");
    driver_lbl.setAlignment(Qt::AlignHCenter | Qt::AlignRight);

    save.setText("Save");

    layout.addWidget(&name_lbl, 0, 0);
    layout.addWidget(&nameInput, 0, 1);
    layout.addWidget(&host_lbl, 1, 0);
    layout.addWidget(&hostInput, 1, 1);
    layout.addWidget(&user_lbl, 2, 0);
    layout.addWidget(&userInput, 2, 1);
    layout.addWidget(&password_lbl, 3, 0);
    layout.addWidget(&passwordInput, 3, 1);
    layout.addWidget(&driver_lbl, 4, 0);
    layout.addWidget(&driverInput, 4, 1);
    layout.addWidget(&save, 5, 0);

    setLayout(&layout);

    connect(&save,&QPushButton::clicked,this,&DBsettings::onSavePressed);
}

void DBsettings::onSavePressed(){
     LocalDataManager::saveDbData(nameInput.text(),hostInput.text(),userInput.text(),passwordInput.text(),driverInput.text());
     emit setMessage("Success","Setting saved!","information");
     emit dataSaved();
 }
