#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QFont font;
//    font.setPointSize(10);
//    a.setFont(font);
    MainWindow w;
    w.show();

    return a.exec();
}
