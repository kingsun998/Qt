#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
int main(int argc, char *argv[])
{
    std::cout<<1;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
