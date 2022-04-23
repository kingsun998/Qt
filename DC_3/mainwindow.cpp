#include "mainwindow.h"
#include "ui_mainwindow.h"
//对目录操作的库
#include <direct.h>
#include <QDir>
#include <usbcanunion.h>
#include <test.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);

    qDebug()<<QTime::currentTime().toString();

    QTabWidget *tb=ui->tabWidget;
    baseInfo *baseinfo=new baseInfo();
    tb->addTab(baseinfo,"参数设置");


    frameDisplay *framedisplay=new frameDisplay();
    tb->addTab(framedisplay,"报文显示");


    BootLoader *bootloader=new BootLoader();
    tb->addTab(bootloader,"程序刷写");

    standarSet *stander=new standarSet();
    tb->addTab(stander,"检测标准设置");

    TestDisplay *testdisplay=new TestDisplay();
    tb->addTab(testdisplay,"双通测试");
//    QFont font;
//    font.setPointSize(11);
//    testdisplay->setFont(font);

    Test* test=new Test();
    tb->addTab(test,"测试");
//    HttpServer::instance().run();

    //检测是否可以写入计时器
    timer=new QTimer();
    timer->setInterval(1000);

    //检测是否可以合并计时器
    mergeFiletimer=new QTimer();
    mergeFiletimer->setInterval(1000*60*settings.mergeFile_interval);
   //单通道发送给表

    //开启表的开始计时器
    connect(&usbcanunion,&UsbCanUnion::setFrameStartTime,framedisplay,&frameDisplay::setStartTime);
    //双通道发送给表
    connect(testdisplay,&TestDisplay::SendToFrame,framedisplay,&frameDisplay::getMessage);
    //检测是否可以写入
    connect(timer,&QTimer::timeout,&dbcontroller,&dbController::CheckWrite);
    //检测是否可以合并文件
    connect(mergeFiletimer,&QTimer::timeout,this,&MainWindow::mergeFileRequest);
    //初始化db服务
    connect(&wzserialport_eight,&WZSerialPort::ReceiveEightPortMes,stander,&standarSet::LoadEightPortMes);
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
    timer->start();
    mergeFiletimer->start();
}
//全局操作，检查是否要合并文件
void MainWindow::mergeFileRequest(){
    for (int i=4;i<settings.dirnames.size();i++) {

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

MainWindow::~MainWindow()
{
    delete ui;
}
