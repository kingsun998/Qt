#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testdisplay.h"
#include "baseinfo.h"
#include "framedisplay.h"
#include "usbcanunion.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);

    baseInfo *baseinfo=new baseInfo();
    ui->tabWidget->addTab(baseinfo,"设置");

    TestDisplay *testdisplay=new TestDisplay();
    ui->tabWidget->addTab(testdisplay,"展示");

    frameDisplay *framedisplay=new frameDisplay();
    ui->tabWidget->addTab(framedisplay,"表格展示");

    connect(testdisplay,&TestDisplay::SendToFrame,framedisplay,&frameDisplay::getMessage);
    //开启表的开始计时器
    connect(&usbcanunion,&UsbCanUnion::setFrameStartTime,framedisplay,&frameDisplay::setStartTime);

    //初始化文件夹
    QString dir=QCoreApplication::applicationDirPath()+"/savefiles/";
    QDir dr;
    if(!QDir(dir).exists()){
        dr.mkdir(dir);
    }
    for (int i=0;i<settings.dirnames.size();i++) {
        QString dirname=QCoreApplication::applicationDirPath()+"/savefiles/"+settings.dirnames[i];
        if(!QDir(dirname).exists()){
            dr.mkdir(dirname);
        }
    }


    //检测是否可以写入
    connect(&timer,&QTimer::timeout,&dbcontroller,&dbController::CheckWrite);
    //检测是否可以合并文件
    connect(&mergeFiletimer,&QTimer::timeout,this,&MainWindow::mergeFileRequest);

    //检测是否可以写入计时器
    mergeFiletimer.setInterval(1000*60*settings.mergeFile_interval);
    timer.setInterval(1000);

    mergeFiletimer.start();
    timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//全局操作，检查是否要合并文件
void MainWindow::mergeFileRequest(){
    for (int i=2;i<settings.dirnames.size();i++) {
        QString basepath=QCoreApplication::applicationDirPath()+"/savefiles/";
        qDebug()<<"check dir"<<basepath+settings.dirnames[i];
        QDir dir(basepath+settings.dirnames[i]);
        //12+2  包含 .  ..
        if(dir.count()>=(settings.mergesize+2+1)){
            dbMergeFile *merge=new dbMergeFile(basepath+settings.dirnames[i]+"/",
                                               basepath+settings.dirnames[i-4]+"/");
            dbcontroller.addObject(merge);
            qDebug()<<"add merge node :"<<settings.dirnames[i];
        }
    }
}
