#include "mainwindow.h"
#include <QApplication>
#include <backend.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Backend server;
    return a.exec();
}
