#include "mainwindow.h"
#include "ui_mainwindow.h"
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

//    SendMessages *sendmessages=new SendMessages();
//    tb->addTab(sendmessages,"发送消息");

    standarSet *stander=new standarSet();
    tb->addTab(stander,"检测标准设置");
    //发送给表
    connect(chart,&chartDisplay::sendMessage,framedisplay,&frameDisplay::getMessage);

    //初始化db服务
//    db.init();
//    qDebug()<<db.ChartThreadIsAlive();
//    qDebug()<<db.TableThreadIsAlive();
}

MainWindow::~MainWindow()
{
    delete ui;
}
