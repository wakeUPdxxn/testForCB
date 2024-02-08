QT       +=core
QT       +=gui
QT       +=network
QT       +=widgets
QT       +=concurrent
QT       +=sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    backend.cpp \
    dbhandler.cpp \
    localdatamanager.cpp \
    main.cpp \
    mainwindow.cpp \
    popup.cpp

HEADERS += \
    backend.h \
    dbhandler.h \
    localdatamanager.h \
    mainwindow.h \
    popup.h

FORMS += \
    mainwindow.ui

target.path: INSTALLS += target
