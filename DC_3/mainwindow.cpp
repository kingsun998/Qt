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
    tb->addTab(baseinfo,"基础设置");

    chartDisplay *chart=new chartDisplay();
    tb->addTab(chart,"图标展示");

    frameDisplay *framedisplay=new frameDisplay();
    tb->addTab(framedisplay,"表格展示");

    SendMessages *sendmessages=new SendMessages();
    tb->addTab(sendmessages,"发送消息");

    standarSet *stander=new standarSet();
    tb->addTab(stander,"设置标准");
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
