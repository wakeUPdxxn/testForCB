QT       +=core
QT       +=gui
QT       +=network
QT       +=widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    backend.cpp \
    dbhandler.cpp \
    localdatamanager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    backend.h \
    dbhandler.h \
    localdatamanager.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

if(qnx){
    qnx: target.path = /tmp/$${TARGET}/bin
}
else:
    unix: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
