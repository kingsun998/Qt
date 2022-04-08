#include "testdisplay.h"
#include "ui_testdisplay.h"
#include "QDateTime"
#include <usbcanunion.h>
#include <dbcontroller.h>
#include "QMessageBox"
#include <time.h>

TestDisplay::TestDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestDisplay)
{
    ui->setupUi(this);
    //使用的名称
    company_usedate=settings.company_usedate_names;
    company_date=settings.company_datenames;

    chart=new Mychart(0,0,0,2,0);
    chartview=new QChartView(chart);
    ui->horizontalLayout_3->addWidget(chartview);

    buttongroup=new QButtonGroup(this);
    buttongroup->addButton(ui->checkBox);
    buttongroup->addButton(ui->checkBox_2);
    //互斥
    buttongroup->setExclusive(false);

    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(true);
    //时间启动器
    timer=new QTimer();
    timer->setInterval(settings.timer_interval);

    saveInterval_miseconds=settings.saveInterval_minus*60*1000;
    //这是index
    m_x=0;
    ReceiveVal.setParameter("A");

    //初始是没有开始运行的
    IfRun=false;
    companyname=settings.CompanyName[0];
    offset=0;

    connect(timer,&QTimer::timeout,this,&TestDisplay::handleTimeOut);
    connect(this,&TestDisplay::sendMessage,this,&TestDisplay::show_detail);
    connect(this,&TestDisplay::sendtochart,chart,&Mychart::getMessage);
    connect(buttongroup,SIGNAL(buttonToggled(int,bool)),this,SLOT(shspline(int,bool)));

    ui->lineEdit_5->setEnabled(false);
    ui->spinBox->setEnabled(false);

    sendtimer=new QTimer();
    connect(sendtimer,&QTimer::timeout,this,&TestDisplay::handleSenderTimeOut);
    sendtimes=0;

    frameConfig={
                 {"SendType",{{"正常发送",0},{"单次发送",1}}},
                 {"RemoteFlag",{{"数据帧",0},{"远程帧",1}}},
                 {"ExternFlag",{{"标准帧",0},{"扩展帧",1}}}
                };
}

TestDisplay::~TestDisplay()
{
    delete ui;
}




void TestDisplay::show_detail(uint mx,CAN_OBJ obj,QString datetime){
    double Tcf,Tcs;
    //发送到表格
    emit SendToFrame(mx,obj,datetime,companyname);

    //计算数值和时间
    Tcf=(obj.Data[0]+obj.Data[1]*256)*0.001;
    Tcs=(obj.Data[2]+obj.Data[3]*256)*0.001;
    emit sendtochart(mx/2,0,Tcf);
    emit sendtochart(mx/2,1,Tcs);
    //保存数据
    ReceiveVal.obj["O°"]["val"].push_back(QString::number(Tcf,'f',2));
    ReceiveVal.obj["HUM"]["val"].push_back(QString::number(Tcs,'f',2));

    ui->label_7->setText(QString::number(Tcf,'f',2));
    ui->label_8->setText(QString::number(Tcs,'f',2));
    // 接受时间
    ReceiveTimestamp.push_back(datetime);

}


Q_DECLARE_METATYPE(QVector<QVector<QString>>)
Q_DECLARE_METATYPE(QVector<QString>)
Q_DECLARE_METATYPE(QVector<QVector<double>>)

//Q_DECLARE_METATYPE(QMap<QString,QMap<QString,QMap<QString,QVector<QString>>>>)
void TestDisplay::handleTimeOut(){
        //这部分是公用的
        double newtime=GetTickCount();

        if(newtime-saveTime>saveInterval_miseconds){
                dbSingleChart *obj=new dbSingleChart(ReceiveVal.obj,
                                                     ReceiveTimestamp);
                ReceiveVal.reset();
                dbcontroller.addObject(obj);

            saveTime=newtime;
            QDateTime end=QDateTime::currentDateTime();
        }
        if(settings.TestMode){
            ULONG len=usbcanunion.StartTestMode();

            for(uint i=0;i<len;i++){
                emit sendMessage(m_x+i,usbcanunion.objs[i],QDateTime::currentDateTime().toString("yyyy_MM_dd hh:mm:ss:zzz"));
            }
            m_x+=len;
        }
        else{
            ULONG len=usbcanunion.ReceiveFromCan();
            qDebug()<<"open success "<<len;
            for(uint i=0;i<len;i++){
                emit sendMessage(m_x+i,usbcanunion.objs[i],QDateTime::currentDateTime().toString("yyyy_MM_dd hh:mm:ss:zzz"));
            }
            m_x+=len;

        }
}

void TestDisplay::shspline(int index,bool selected){
    index=-index-2;
    chart->show_hidden_Series(index,selected);
}

//
void TestDisplay::connectCan(){

    if(settings.TestMode){
        qDebug()<<"测试模式不用链接硬件，可以直接开启测试";
        settings.caninit=true;
    }else{
        DWORD drl;
        drl=usbcanunion.openUsbCan();
        if (drl != STATUS_OK)
        {
            QMessageBox::information(this,"提示","链接失败",QMessageBox::Warning);
            return ;
        }
        drl=usbcanunion.initUsbCan();
        if (drl != STATUS_OK)
        {
            QMessageBox::information(this,"提示","初始化CAN失败",QMessageBox::Warning);
            return ;
        }
        drl=usbcanunion.startUsbCan();
        if (drl != STATUS_OK)
        {
            QMessageBox::information(this,"提示","开启失败",QMessageBox::Warning);
            return ;
        }
        settings.caninit=true;
    }

}
//pushbuttons[2]
void TestDisplay::startCan(){
    if(settings.TestMode){
        if(IfRun==false){
            //    测试代码
            IfRun=true;
            emit usbcanunion.setFrameStartTime();
            timer->start(settings.timer_interval);
            saveTime=GetTickCount();
            ui->pushButton_3->setText("停止测试");
        }
        else{
            IfRun=false;
            timer->stop();
            ui->pushButton_3->setText("开启测试");
        }
    }
    else{
        if(IfRun==false){
            IfRun=true;
            emit usbcanunion.setFrameStartTime();
            timer->start(100);
            //重新技术
            saveTime=GetTickCount();
            ui->pushButton_3->setText("停止测试");
        }else{
            IfRun=false;
            timer->stop();
            ui->pushButton_3->setText("开启测试");
        }
    }
}

void TestDisplay::disconnectCan(){
    if(settings.TestMode){
        qDebug()<<"测试模式无需关闭链接";
    }else{
        DWORD drl;
        drl=usbcanunion.closeUsbCan();
        if (drl != STATUS_OK)
        {
            QMessageBox::information(this,"提示","关闭链接失败",QMessageBox::Warning);
            return ;
        }
        settings.caninit=false;
    }
}

void TestDisplay::YscaleJump(){
    double ymin=ui->lineEdit->text().toDouble();
    double ymax=ui->lineEdit_2->text().toDouble();
    qDebug()<<ymin<<"  "<<ymax;
    chart->changeYScale(ymin,ymax);
}



void TestDisplay::on_pushButton_clicked()
{
    this->YscaleJump();
}

//关闭can
void TestDisplay::on_pushButton_4_clicked()
{
    this->disconnectCan();
}

void TestDisplay::on_pushButton_2_clicked()
{
    this->connectCan();
}

void TestDisplay::on_pushButton_3_clicked()
{
    this->startCan();
}

void TestDisplay::on_pushButton_5_clicked()
{
    if(ui->radioButton->isChecked()){
        sendtimes=ui->lineEdit_5->text().toUInt();
        sendtimer->setInterval(ui->spinBox->text().toInt());
        sendtimer->start();
    }
    else{
        sendSingleFrameToCan();
    }
}

void TestDisplay::sendSingleFrameToCan()
{
    CAN_OBJ obj;
    QStringList slist=ui->lineEdit_3->text().split(" ");
    for(int i=0;i<slist.size();i++){
        obj.Data[i]=slist[i].toUInt(nullptr,16);
    }
    qDebug()<<ui->lineEdit_4->text();
    obj.ID=ui->lineEdit_4->text().toUInt(nullptr,16);
    obj.SendType=frameConfig["SendType"][ui->comboBox_3->currentText()];
    obj.RemoteFlag=frameConfig["RemoteFlag"][ui->comboBox_2->currentText()];
    obj.ExternFlag=frameConfig["ExternFlag"][ui->comboBox->currentText()];
    obj.DataLen=8;
    qDebug()<<obj.SendType<<" "<<obj.RemoteFlag<<" "<<obj.ExternFlag;
    qDebug()<<obj.ID;
    for(int i=0;i<8;i++){
        qDebug()<<obj.Data[i];
    }
    usbcanunion.SendToCan(obj);
}

void TestDisplay::handleSenderTimeOut(){
    if(sendtimes==0){
        sendtimer->stop();
        return;
    }
    sendSingleFrameToCan();
    sendtimes-=1;
}

void TestDisplay::on_radioButton_clicked(bool checked)
{
    ui->lineEdit_5->setEnabled(checked);
    ui->spinBox->setEnabled(checked);
}
