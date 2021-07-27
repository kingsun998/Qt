#include "chartdisplay.h"
#include "ui_chartdisplay.h"



chartDisplay::chartDisplay(QWidget *parent):
    ui(new Ui::chartdisplay_ui),
    m_x(0)
{
    ui->setupUi(this);
    fchart=new Mychart(0,0,0,4);
    schart=new Mychart(0,0,1,3);
    fchartView=new QChartView(fchart);
    schartView=new QChartView(schart);
    ui->chartLayout->addWidget(fchartView);
    ui->chartLayout->addWidget(schartView);


    //测试开关默认关闭
    iftest=false;


    //时间启动器
    timer=new QTimer();
    timer->setInterval(250);
    connect(timer,&QTimer::timeout,this,&chartDisplay::handleTimeOut);

    saveInterval_miseconds=settings.saveInterval_minus*60*1000;
    //设置多选按钮
    btg=new QButtonGroup();
    qDebug()<<1;
    for(int i=0;i<7;i++){
        pb[i]=new QCheckBox();
        pb[i]->setText(settings.splineName[i]);
        ui->verticalLayout_5->addWidget(pb[i],1);
        btg->addButton(pb[i]);
        timestart[i]=0;
        timeend[i]=0;
        records_bt[i]=false;
        records_tp[i]=false;
        //设置vector大小
    }
    saveTempeture.resize(settings.lineNums);
    saveTimestamp.resize(settings.lineNums);
    status.resize(settings.lineNums);
    time.resize(settings.lineNums);
    // 设置不互斥
    btg->setExclusive(false);//这样的话就支持多选了。
    //设置多选按钮
    connect(btg,SIGNAL(buttonToggled(int,bool)),this,SLOT(shspline(int,bool)));

    //设置展示细节
    connect(this,SIGNAL(sendMessage(uint,CAN_OBJ,QString)),this,SLOT(show_detail(uint,CAN_OBJ,QString)));
    //发送给表定义在mainwindow

    //定义图标的槽函数
    connect(this,&chartDisplay::sendtochart,fchart,&Mychart::getMessage);
    connect(this,&chartDisplay::sendtochart,schart,&Mychart::getMessage);

}

chartDisplay::~chartDisplay(){

}
Q_DECLARE_METATYPE(QVector<QVector<QString>>)
Q_DECLARE_METATYPE(QVector<QVector<double>>)
void chartDisplay::handleTimeOut(){
        double newtime=GetTickCount();
        if(newtime-saveTime>saveInterval_miseconds){
            qDebug()<<"主线程："<<QThread::currentThreadId();
            QDateTime start=QDateTime::currentDateTime();
            qDebug()<<"开始"<<start.toUTC();
            QVector<QVector<double>> save_mid_temperature;
            QVector<QVector<QString>> save_mid_timestamp;
            QVector<QVector<QString>> save_mid_status;
            QVector<QVector<double>> save_mid_time;
            //swap 速度快，且清空
            save_mid_temperature.swap(saveTempeture);
            save_mid_timestamp.swap(saveTimestamp);
            save_mid_status.swap(status);
            save_mid_time.swap(time);
            saveTempeture.resize(settings.lineNums);
            saveTimestamp.resize(settings.lineNums);
            status.resize(settings.lineNums);
            time.resize(settings.lineNums);
            qDebug()<<"数组大小"<<save_mid_time.size();
            qDebug()<<"数组大大小"<<save_mid_time[0].size();
            QVariant tp=QVariant::fromValue(save_mid_temperature);
//            tp.setValue(save_mid_temperature);
            QVariant ts=QVariant::fromValue(save_mid_timestamp);
//            ts.setValue(save_mid_temperature);
            QVariant sta=QVariant::fromValue(save_mid_status);
//            sta.setValue(save_mid_temperature);
            QVariant tm=QVariant::fromValue(save_mid_time);
//            tp.setValue(save_mid_temperature);
            emit db.saveChart(tp,ts,sta,tm,QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
            saveTime=newtime;
            QDateTime end=QDateTime::currentDateTime();
            qDebug()<<"结束"<<end.toUTC();
        }

        ULONG len=Receive(settings.devicetype,settings.deviceid,settings.canid,objs,50,100);
//        qDebug()<<"接受数据了";
        for(uint i=0;i<len;i++){
            emit sendMessage(m_x+i,objs[i],QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.zzz_ddd"));
        }
        m_x+=len;
}


double chartDisplay::Calculate(int index,double y){
    //表示没有记录温度
    if(y<settings.Bt_temperature){
        records_bt[index]=false;
        return -1;
    }
    //记录了初始温度
    if(!records_bt[index]&&y>settings.Bt_temperature){
        timestart[index]=GetTickCount();
        records_bt[index]=true;
    }
    if(y<settings.Tp_temperature){
        records_tp[index]=false;
        return -1;
    }
    if(!records_tp[index]&&y>settings.Tp_temperature){
        timeend[index]=GetTickCount();
        records_tp[index]=true;
        return (timeend[index]-timestart[index])/1000;
    }
    return -1;
}


void chartDisplay::show_detail(uint mx,CAN_OBJ obj,QString datetime){
    double low;
    double high;
    double Tcf;
    double Tcs;
    int error1=0;
    int error2=0;
    int base=1;
    int tmp;
    double timetemp;

//    mx/=4;
    switch (obj.ID) {
    //18FD2083
        case 419242115:
            low=obj.Data[0];
            high=obj.Data[1];
            Tcf=(high*256+low)*0.03125-273;
            low=obj.Data[2];
            high=obj.Data[3];
            Tcs=(high*256+low)*0.03125-273;
            emit sendtochart(0,mx/4,0,Tcf,Tcs);
            ui->label->setText(QString::number(Tcf));
            ui->label_2->setText(QString::number(Tcs));

            //计算时间
            timetemp=Calculate(0,Tcf);
            //不等于 -1
            if(timetemp+1>1e-6){
                ui->label_8->setText(QString::number(timetemp));
            }
            time[0].push_back(timetemp);
            timetemp=Calculate(1,Tcs);
            if(timetemp+1>1e-6){
                ui->label_9->setText(QString::number(timetemp));
            }
            time[1].push_back(timetemp);
            //计算错误码
            for(int i=0;i<5;i++){
                tmp=obj.Data[6]>>i&1;
                error1+=base*tmp;
                base*=2;
            }
            ui->label_15->setText(settings.errorCode_TC[error1]);
            base=1;
            for(int i=5;i<8;i++){
                tmp=obj.Data[6]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            for(int i=0;i<2;i++){
                tmp=obj.Data[7]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            ui->label_21->setText(settings.errorCode_TC[error2]);
            //保存数据
//            saveTempeture[0].size();
            saveTempeture[0].push_back(Tcf);
            saveTempeture[1].push_back(Tcs);
            saveTimestamp[0].push_back(datetime);
            saveTimestamp[1].push_back(datetime);
            status[0].push_back(settings.errorCode_TC[error1]);
            status[1].push_back(settings.errorCode_TC[error2]);

            break;
        case 352140931:
            low=obj.Data[0];
            high=obj.Data[1];
            Tcf=(high*256+low)*0.03125-273;
            low=obj.Data[3];
            high=obj.Data[4];
            Tcs=(high*256+low)*0.03125-273;
            emit sendtochart(0,mx/4,1,Tcf,Tcs);
            ui->label_3->setText(QString::number(Tcf));
            ui->label_4->setText(QString::number(Tcs));

            //计算时间
            timetemp=Calculate(0,Tcf);
            //不等于 -1
            if(timetemp+1>1e-6){
                ui->label_10->setText(QString::number(timetemp));
            }
            time[2].push_back(timetemp);
            timetemp=Calculate(1,Tcs);
            if(timetemp+1>1e-6){
                ui->label_11->setText(QString::number(timetemp));
            }
            time[3].push_back(timetemp);
            //计算错误码
            for(int i=0;i<5;i++){
                tmp=obj.Data[2]>>i&1;
                error1+=base*tmp;
                base*=2;
            }
            ui->label_20->setText(settings.errorCode_TC[error1]);
            base=1;
            for(int i=1;i<5;i++){
                tmp=obj.Data[5]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            ui->label_19->setText(settings.errorCode_TC[error2]);
            //保存数据
            saveTempeture[2].push_back(Tcf);
            saveTempeture[3].push_back(Tcs);
            saveTimestamp[2].push_back(datetime);
            saveTimestamp[3].push_back(datetime);
            status[2].push_back(settings.errorCode_TC[error1]);
            status[3].push_back(settings.errorCode_TC[error2]);
            break;
        case 419280003:
            low=obj.Data[2];
            high=obj.Data[3];
            Tcf=(high*256+low)*0.03125-273;
            emit sendtochart(1,mx/4,2,Tcf,-1);
            ui->label_5->setText(QString::number(Tcf));

            //计算时间
            timetemp=Calculate(0,Tcf);
            //不等于 -1
            if(timetemp+1>1e-6){
                ui->label_12->setText(QString::number(timetemp));
            }
             time[4].push_back(timetemp);
            //计算错误码
            for(int i=1;i<5;i++){
                tmp=obj.Data[5]>>i&1;
                error1+=base*tmp;
                base*=2;
            }
            ui->label_18->setText(settings.errorCode_TC[error1]);
            //保存数据
            saveTempeture[4].push_back(Tcf);
            saveTimestamp[4].push_back(datetime);
            status[4].push_back(settings.errorCode_TC[error1]);
            break;
        case 419395027:
            low=obj.Data[0];
            high=obj.Data[1];
            Tcf=(high*256+low)*0.03125-273;
            low=obj.Data[2];
            high=obj.Data[3];
            Tcs=(high*256+low)*0.03125-273;
            emit sendtochart(1,mx/4,3,Tcf,Tcs);
            ui->label_6->setText(QString::number(Tcf));
            ui->label_7->setText(QString::number(Tcs));

            //计算时间
            timetemp=Calculate(0,Tcf);
            //不等于 -1
            if(timetemp+1>1e-6){
                ui->label_13->setText(QString::number(timetemp));
            }
            time[5].push_back(timetemp);
            timetemp=Calculate(1,Tcs);
            if(timetemp+1>1e-6){
                ui->label_14->setText(QString::number(timetemp));
            }
            time[6].push_back(timetemp);
            //计算错误码
            for(int i=0;i<5;i++){
                tmp=obj.Data[6]>>i&1;
                error1+=base*tmp;
                base*=2;
            }
            ui->label_17->setText(settings.errorCode_CJ[error1]);
            base=1;
            for(int i=5;i<8;i++){
                tmp=obj.Data[6]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            for(int i=0;i<2;i++){
                tmp=obj.Data[7]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            ui->label_16->setText(settings.errorCode_ECU[error2]);
            //保存数据
            saveTempeture[5].push_back(Tcf);
            saveTempeture[6].push_back(Tcs);
            saveTimestamp[5].push_back(datetime);
            saveTimestamp[6].push_back(datetime);
            status[5].push_back(settings.errorCode_TC[error1]);
            status[6].push_back(settings.errorCode_TC[error2]);
            break;
        default: break;
    }
}


void chartDisplay::shspline(int index,bool selected){
    index=-index-2;

    if(index<4){
       fchart->show_hidden_Series(index,selected);
    }else{
       schart->show_hidden_Series(index-4,selected);
    }
}

// 链接硬件
void chartDisplay::on_pushButton_clicked()
{
    DWORD drl;
    drl=OpenDevice(settings.devicetype,settings.deviceid,0);
    if (drl != STATUS_OK)
    {
        QMessageBox::information(this,"提示","链接失败",QMessageBox::Warning);
        return ;
    }
    drl=InitCAN(settings.devicetype,settings.deviceid,settings.canid,&settings.pInitConfig);
    if (drl != STATUS_OK)
    {
        QMessageBox::information(this,"提示","初始化CAN失败",QMessageBox::Warning);
        return ;
    }
}

//关闭硬件
void chartDisplay::on_pushButton_3_clicked()
{
    DWORD drl;
    drl=CloseDevice(settings.devicetype,settings.deviceid);
    if (drl != STATUS_OK)
    {
        QMessageBox::information(this,"提示","关闭链接失败",QMessageBox::Warning);
        return ;
    }
}

//控制测试的开始和暂停
void chartDisplay::on_pushButton_2_clicked(bool checked)
{
    if(iftest==false){
        DWORD drl;
        drl=StartCAN(settings.devicetype, settings.deviceid, settings.canid);
        if (drl != STATUS_OK)
        {
            QMessageBox::information(this,"提示","开启失败",QMessageBox::Warning);
            return ;
        }else{
            iftest=true;
            timer->start(100);
            //重新技术
            saveTime=GetTickCount();
            ui->pushButton_2->setText("停止测试");
        }

    }else{
        iftest=false;
        timer->stop();
        ui->pushButton_2->setText("开启测试");
    }
}

void chartDisplay::on_pushButton_2_clicked()
{
}

void chartDisplay::on_pushButton_4_clicked()
{
    fchart->changeYScale(ui->lineEdit_2->text().toDouble(),ui->lineEdit->text().toDouble());
    schart->changeYScale(ui->lineEdit_2->text().toDouble(),ui->lineEdit->text().toDouble());
}
