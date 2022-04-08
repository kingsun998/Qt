#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <direct.h>
#include <dbcontroller.h>
#include <usbcanunion.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);
    QTabWidget *tb=ui->tabWidget;
    baseInfo *baseinfo=new baseInfo();
    tb->addTab(baseinfo,"参数设置");

    chartDisplay *chart=new chartDisplay();
    tb->addTab(chart,"曲线显示");

    frameDisplay *framedisplay=new frameDisplay();
    tb->addTab(framedisplay,"报文显示");

    BootLoader *bootloader=new BootLoader();
    tb->addTab(bootloader,"程序刷写");

    standarSet *stander=new standarSet();
    tb->addTab(stander,"检测标准设置");

    timer=new QTimer();
    timer->setInterval(1000);

    //检测是否可以合并计时器
    mergeFiletimer=new QTimer();
    mergeFiletimer->setInterval(1000*60*settings.mergeFile_interval);
    //开启表的开始计时器
    connect(&usbcanunion,&UsbCanUnion::setFrameStartTime,framedisplay,&frameDisplay::setStartTime);
    //发送给表
    connect(chart,&chartDisplay::sendMessage,framedisplay,&frameDisplay::getMessage);
    connect(chart,&chartDisplay::sendsingleframetocan,framedisplay,&frameDisplay::getMessage);

    //检测是否可以写入
    connect(timer,&QTimer::timeout,&dbcontroller,&dbController::CheckWrite);
    //检测是否可以合并文件
    connect(mergeFiletimer,&QTimer::timeout,this,&MainWindow::mergeFileRequest);

    QString dir=QCoreApplication::applicationDirPath()+"/savefiles/";

    QDir dr;
    if(!QDir(dir).exists()){
        dr.mkdir(dir);
    }
    for (int i=0;i<settings.dirlist.size();i++) {
        QString dirname=QCoreApplication::applicationDirPath()+"/savefiles/"+settings.dirlist[i];
        if(!QDir(dirname).exists()){
            dr.mkdir(dirname);
        }
    }
    timer->start();
    mergeFiletimer->start();
}

//全局操作，检查是否要合并文件
void MainWindow::mergeFileRequest(){
    for (int i=2;i<settings.dirlist.size();i++) {

        QString basepath=QCoreApplication::applicationDirPath()+"/savefiles/";
        qDebug()<<"check dir"<<basepath+settings.dirlist[i];
        QDir dir(basepath+settings.dirlist[i]);
        //12+2  包含 .  ..
        if(dir.count()>=(settings.mergesize+2+1)){
            dbMergeFile *merge=new dbMergeFile(basepath+settings.dirlist[i]+"/",
                                               basepath+settings.dirlist[i-2]+"/");
            dbcontroller.addObject(merge);
            qDebug()<<"add merge node :"<<settings.dirlist[i];
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
