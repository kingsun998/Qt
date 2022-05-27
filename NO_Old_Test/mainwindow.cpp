#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <settings.h>
#include <qdebug.h>
#include <utils.h>
#include <mode_scan.h>
#include <mode_sending.h>
#include <mode_settings.h>
#include <mode_upload.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //首先初始化设置
    settings.initConfiguration();
    // 初始化工具栏
    initToolBar();

    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->mainToolBar->setIconSize(QSize(30,30));
    ui->mainToolBar->addActions(toollist);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initToolBar(){
     toollist.append(new QAction(QIcon("images/settings.jpg"),QString::fromLocal8Bit("设置")));
     toollist.append(new QAction(QIcon("images/scan.jpeg"),QString::fromLocal8Bit("扫描枪")));
     toollist.append(new QAction(QIcon("images/sending.jpeg"),QString::fromLocal8Bit("发送数据")));
     toollist.append(new QAction(QIcon("images/upload.jpeg"),QString::fromLocal8Bit("上传")));
     QList<mode_base*> list={
         new mode_settings(this),new mode_scan(this),new mode_sending(this),new mode_upload(this)
     };

     for (int i=0;i<toollist.size();i++) {
         list[i]->init();
         connect(toollist[i],&QAction::triggered,this,[list,i](bool checked)
         {
           list[i]->reset();
           list[i]->show();
         });
     }
}
