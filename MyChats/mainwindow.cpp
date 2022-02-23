#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <winsock2.h>
//#include <winsock.h>
#pragma comment(lib, "ws2_32")
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    WSADATA wsd;
    WSAStartup(MAKEWORD(2,2),&wsd);
}

MainWindow::~MainWindow()
{
    delete ui;
}
