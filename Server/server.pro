QT       +=core
QT       +=gui
QT       +=network
QT       +=widgets
QT       +=concurrent
QT       +=sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

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

system("mkdir RELEASE.absolute_path()/configs")
target.path: INSTALLS += target
