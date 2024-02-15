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
    popup.cpp \
    dbsettings.cpp

HEADERS += \
    backend.h \
    dbhandler.h \
    localdatamanager.h \
    mainwindow.h \
    popup.h \
    dbsettings.h

FORMS += \
    mainwindow.ui

RELEASE.DESTDIR = release;
RELEASE.DEBUG = debug;

system("mkdir release.absolute_path()/configs") #release test conf
system("mkdir debug.absolute_path()/configs") #debuf test conf

target.path: INSTALLS += target
