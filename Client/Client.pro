QT       += core
QT       +=gui
QT       +=network
QT       +=concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    client.cpp \
    connectionsettings.cpp \
    localdatamanager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    client.h \
    connectionsettings.h \
    localdatamanager.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

target.path: INSTALLS += target
