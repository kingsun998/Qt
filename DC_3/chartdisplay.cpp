#include "chartdisplay.h"
#include "ui_chartdisplay.h"
#include <direct.h>
chartDisplay::chartDisplay(QWidget *parent):
    ui(new Ui::chartdisplay_ui),
    m_x(0)
{
    ui->setupUi(this);
    //设置需要展示类型
    companytypecode=settings.CompanyType;
    //
    fchart=new Mychart(0,0,0,4,companytypecode);
    schart=new Mychart(0,0,1,3,companytypecode);
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
    //设置大小
    timestart.resize(settings.lineNums);
    records_bt.resize(settings.lineNums);
    timeend.resize(settings.lineNums);
    records_tp.resize(settings.lineNums);

    saveTempeture.resize(settings.totalnums);
//    saveTimestamp.resize(settings.totalnums);
    status.resize(settings.totalnums);
    time.resize(settings.totalnums);

    pb.resize(settings.totalnums);
    ReceiveVal.resize(settings.totalnums);
    ReceiveTime.resize(settings.totalnums);
    ReceiveStatus.resize(settings.totalnums);

    for(int i=0;i<settings.totalnums;i++){
        pb[i]=new QCheckBox();
        pb[i]->setText(settings.splineName[companytypecode][i]);
        vboxlayout[3].addWidget(pb[i]);
        btg->addButton(pb[i]);
        timestart[i]=0;
        timeend[i]=0;
        pb[i]->setCheckState(Qt::Checked);
        records_bt[i]=false;
        records_tp[i]=false;

        ReceiveVal[i]=new QLabel();
        ReceiveTime[i]=new QLabel();
        ReceiveStatus[i]=new QLabel();

        vboxlayout[0].addWidget(ReceiveVal[i]);
        vboxlayout[1].addWidget(ReceiveStatus[i]);
        vboxlayout[2].addWidget(ReceiveTime[i]);
    }
    groupbox[0].setTitle("数值");
    groupbox[0].setMinimumWidth(100);
    groupbox[0].setLayout(&vboxlayout[0]);
    groupbox[1].setTitle("状态");
    groupbox[1].setMinimumWidth(100);
    groupbox[1].setLayout(&vboxlayout[1]);
    groupbox[2].setTitle("时间");
    groupbox[2].setMinimumWidth(100);
    groupbox[2].setLayout(&vboxlayout[2]);
    groupbox[3].setTitle("曲线");
    groupbox[3].setMinimumWidth(100);
    groupbox[3].setLayout(&vboxlayout[3]);

    ui->horizontalLayout_2->addWidget(&groupbox[3]);
    ui->horizontalLayout_2->addWidget(&groupbox[0]);
    ui->horizontalLayout_2->addWidget(&groupbox[1]);
    ui->horizontalLayout_2->addWidget(&groupbox[2]);
    // 设置不互斥
    btg->setExclusive(false);//这样的话就支持多选了。
    QList<QAbstractButton *> list=btg->buttons();
    for(int i=0;i<settings.lineNums;i++){
        list[i]->setChecked(true);
    }
    qDebug()<<settings.lineNums<<"  "<<settings.totalnums;
    for(int i=settings.lineNums;i<settings.totalnums;i++){
        list[i]->setEnabled(false);
    }
    list[settings.totalnums-1]->setChecked(false);
    //设置多选按钮
    connect(btg,SIGNAL(buttonToggled(int,bool)),this,SLOT(shspline(int,bool)));

    //设置展示细节
    connect(this,&chartDisplay::sendMessage,this,&chartDisplay::show_detail);
    //发送给表定义在mainwindow

    //定义图标的槽函数
    connect(this,&chartDisplay::sendtochart,fchart,&Mychart::getMessage);
    connect(this,&chartDisplay::sendtochart,schart,&Mychart::getMessage);

    ui->comboBox->setMaximumWidth(100);

    QString dir=QCoreApplication::applicationDirPath()+"/savefiles/";
    QString chartdir=QCoreApplication::applicationDirPath()+"/savefiles/charts/";
    QString framedir=QCoreApplication::applicationDirPath()+"/savefiles/frames/";
    QDir Dir(dir);
    if (!Dir.exists())
    {
        qDebug()<<"__________create dir_______________";
        _mkdir(dir.toLatin1().data());
        _mkdir(chartdir.toLatin1().data());
        _mkdir(framedir.toLatin1().data());
    }

    PrecesionMode=false;
    TestTemperature=0;
    offset=0;
}

chartDisplay::~chartDisplay(){

}

void chartDisplay::resetAry(){
    //下标
    m_x=0;
    //接受的总帧数
    totalnums=0;

    for(int i=0;i<settings.lineNums;i++){
        timestart[i]=0;
        records_bt[i]=false;
        timeend[i]=0;
        records_tp[i]=false;
    }
    for(int i=0;i<settings.totalnums;i++){
        saveTempeture[i].clear();

        status[i].clear();
        time[i].clear();
    }
    saveTimestamp.clear();

    saveTempeture.resize(settings.totalnums);
    status.resize(settings.totalnums);
    time.resize(settings.totalnums);
}

void chartDisplay::Calculate(int index,double y){
    //表示没有记录温度
    if(y<settings.Bt_temperature){
        records_bt[index]=false;
    }
    //记录了初始温度
    if(!records_bt[index]&&y>settings.Bt_temperature){
        timestart[index]=GetTickCount();
        records_bt[index]=true;
    }
    if(y<settings.Tp_temperature){
        records_tp[index]=false;
    }
    if(!records_tp[index]&&y>settings.Tp_temperature){
        timeend[index]=GetTickCount();
//        qDebug()<<timeend[index]<<" "<<timestart[index];
//        qDebug()<<records_tp[index]<<" "<<records_bt[index];
        records_tp[index]=true;
    }
    double tp=(timeend[index]-timestart[index])/1000;
    time[index].push_back(tp>0?tp:0);
}

void chartDisplay::suit_Cell(int chartype,int code,uint mx,int index1,int low1,int high1,int index2,int low2,int high2,BYTE *Data){
//    qDebug()<<"msg  4";
    int low=Data[low1];
    int high=Data[high1];
    double Tcf=(high*256+low)*0.03125-273;
    //保存数据
    saveTempeture[index1].push_back(Tcf);
    //计算时间
    Calculate(index1,Tcf);
    ReceiveTime[index1]->setText(QString::number(time[index1].last()));
    ReceiveVal[index1]->setText(QString::number(Tcf));
    if(PrecesionMode){
        QPalette pf;
        bool flg;
        if(offset<0){
            flg=abs(Tcf-TestTemperature)<TestTemperature*(-offset);
        }else{
            flg=abs(Tcf-TestTemperature)<1.5;
        }
        pf.setColor(QPalette::WindowText,settings.testColor[flg]);
        ReceiveVal[index1]->setPalette(pf);
    }

    //排除第三个
    if(index2==-1){
        emit sendtochart(1,mx/4,2,Tcf,-1,-1);
        return ;
    }
    low=Data[low2];
    high=Data[high2];
    double Tcs=(high*256+low)*0.03125-273;
    saveTempeture[index2].push_back(Tcs);
    Calculate(index2,Tcs);
    ReceiveTime[index2]->setText(QString::number(time[index2].last()));
    ReceiveVal[index2]->setText(QString::number(Tcs));
//    qDebug()<<"msg  6";
    //若已经开启测试模式，则变换其颜色
    if(PrecesionMode){
        QPalette ps;
        bool flg;
        if(offset<0){
            flg=abs(Tcs-TestTemperature)<TestTemperature*(-offset);
        }else{
            flg=abs(Tcs-TestTemperature)<1.5;
        }
        ps.setColor(QPalette::WindowText,settings.testColor[flg]);
        ReceiveVal[index2]->setPalette(ps);
    }
    emit sendtochart(chartype,mx/4,code,Tcf,Tcs,-1);

}

void chartDisplay::show_detail(uint mx,CAN_OBJ obj,QString datetime,int companycode){
    int error1=0;
    int error2=0;
    int base=1;
    int tmp;
    double V1,V2,V3,V4;
    double Tcf,Tcs,Tct;
//    qDebug()<<"msg 3   "<<obj.ID;
//    qDebug()<<"msg 3   "<<datetime;
    switch (obj.ID) {
        case 419395283: //18ff76d3
        case 419242115://18FD2083    这两帧处理方式一样
            //计算数值和时间
            suit_Cell(0,0,mx,0,0,1,1,2,3,obj.Data);
            //计算错误码
            for(int i=0;i<5;i++){
                tmp=obj.Data[6]>>i&1;
                error1+=base*tmp;
                base*=2;
            }

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
            qDebug()<<1<<"  "<<error1<<"   "<<error2;
            ReceiveStatus[0]->setText(settings.errorCode_TC[error1]);
            ReceiveStatus[1]->setText(settings.errorCode_TC[error2]);
            status[0].push_back(settings.errorCode_TC[error1]);
            status[1].push_back(settings.errorCode_TC[error2]);

            saveTimestamp.push_back(datetime);
            break;
        case 352140931:  //14fd3e83
            suit_Cell(0,1,mx,2,0,1,3,3,4,obj.Data);

            //计算错误码
            for(int i=0;i<5;i++){
                tmp=obj.Data[2]>>i&1;
                error1+=base*tmp;
                base*=2;
            }
            ReceiveStatus[2]->setText(settings.errorCode_TC[error1]);
            base=1;
            for(int i=1;i<5;i++){
                tmp=obj.Data[5]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            ReceiveStatus[3]->setText(settings.errorCode_TC[error2]);
            qDebug()<<error1<<"   "<<error2;
            status[2].push_back(settings.errorCode_TC[error1]);
            status[3].push_back(settings.errorCode_TC[error2]);
            break;
        case 419280003:  //18fdb483
            suit_Cell(1,2,mx,4,2,3,-1,-1,-1,obj.Data);
            //计算错误码
            for(int i=1;i<5;i++){
                tmp=obj.Data[5]>>i&1;
                error1+=base*tmp;
                base*=2;
            }
            ReceiveStatus[4]->setText(settings.errorCode_TC[error1]);
            saveTimestamp.push_back(datetime);
            status[4].push_back(settings.errorCode_TC[error1]);
            break;
        case 419395027:  //18ff75d3
            if(companytypecode==0){
                suit_Cell(1,3,mx,5,0,1,6,2,3,obj.Data);
            }else if(companytypecode==1){
                suit_Cell(0,1,mx,2,0,1,3,2,3,obj.Data);
            }
            //计算错误码
            for(int i=0;i<5;i++){
                tmp=obj.Data[6]>>i&1;
                error1+=base*tmp;
                base*=2;
            }
            if(companytypecode==0){
                ReceiveStatus[5]->setText(settings.errorCode_CJ[error1]);
            }else if(companytypecode==1){
                ReceiveStatus[2]->setText(settings.errorCode_CJ[error1]);
            }
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
            qDebug()<<2<<"  "<<error1<<"   "<<error2;
            if(companytypecode==0){
                ReceiveStatus[6]->setText(settings.errorCode_ECU[error2]);
                status[5].push_back(settings.errorCode_ECU[error1]);
                status[6].push_back(settings.errorCode_ECU[error2]);
            }else{
                ReceiveStatus[3]->setText(settings.errorCode_ECU[error2]);
                status[2].push_back(settings.errorCode_ECU[error1]);
                status[3].push_back(settings.errorCode_ECU[error2]);
            }
            break;
        case 285034371:   //10fd4783
            V1=(obj.Data[1]*256+obj.Data[0])*1.0/200-100;
            V2=(obj.Data[3]*256+obj.Data[2])*1.0/200-100;
            V3=(obj.Data[5]*256+obj.Data[4])*1.0/200-100;
            V4=(obj.Data[7]*256+obj.Data[6])*1.0/200-100;
            ReceiveVal[7]->setText(QString::number(V1));
            ReceiveVal[8]->setText(QString::number(V1));
            ReceiveVal[9]->setText(QString::number(V1));
            ReceiveVal[10]->setText(QString::number(V1));
            saveTempeture[7].push_back(V1);
            saveTempeture[8].push_back(V2);
            saveTempeture[9].push_back(V3);
            saveTempeture[10].push_back(V4);

            break;

        case 419394771:  //18ff74d3   //特殊帧
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            //保存数据
            saveTempeture[4].push_back(Tcf);
            //计算时间
            Calculate(4,Tcf);
            ReceiveTime[4]->setText(QString::number(time[4].last()));
            ReceiveVal[4]->setText(QString::number(Tcf));
            for(int i=0;i<5;i++){
                tmp=obj.Data[6]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            ReceiveStatus[4]->setText(settings.errorCode_TC[error2]);
            status[4].push_back(settings.errorCode_TC[error2]);

            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;
            saveTempeture[5].push_back(Tcs);
            Calculate(5,Tcs);
            ReceiveTime[5]->setText(QString::number(time[5].last()));
            ReceiveVal[5]->setText(QString::number(Tcs));
            error2=0;
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
            ReceiveStatus[5]->setText(settings.errorCode_TC[error2]);
            status[5].push_back(settings.errorCode_TC[error2]);


            Tct=(obj.Data[5]*256+obj.Data[4])*0.03125-273;
            saveTempeture[6].push_back(Tct);
            Calculate(6,Tct);
            ReceiveTime[6]->setText(QString::number(time[6].last()));
            ReceiveVal[6]->setText(QString::number(Tct));
            error2=0;
            base=1;
            for(int i=2;i<7;i++){
                tmp=obj.Data[7]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            ReceiveStatus[6]->setText(settings.errorCode_TC[error2]);
            status[6].push_back(settings.errorCode_TC[0]);
            base=1;

            emit sendtochart(1,mx/4,4,Tcf,Tcs,Tct);
        break;
        case 419395539:  //18ff77d3 此帧结果等于  18ff75d3 此处忽略

        break;
        default: break;
    }
}


Q_DECLARE_METATYPE(QVector<QVector<QString>>)
Q_DECLARE_METATYPE(QVector<QString>)
Q_DECLARE_METATYPE(QVector<QVector<double>>)
void chartDisplay::handleTimeOut(){
        double newtime=GetTickCount();
        if(newtime-saveTime>saveInterval_miseconds){
            qDebug()<<"主线程："<<QThread::currentThreadId();
            QDateTime start=QDateTime::currentDateTime();
            qDebug()<<"开始"<<start.toUTC();
            QVector<QVector<double>> save_mid_temperature;
            QVector<QString> save_mid_timestamp;
            QVector<QVector<QString>> save_mid_status;
            QVector<QVector<double>> save_mid_time;
            //swap 速度快，且清空
            save_mid_temperature.swap(saveTempeture);
            save_mid_timestamp.swap(saveTimestamp);
            save_mid_status.swap(status);
            save_mid_time.swap(time);
            saveTempeture.resize(settings.totalnums);
//            saveTimestamp.resize(settings.totalnums);
            status.resize(settings.totalnums);
            time.resize(settings.totalnums);
            qDebug()<<"数组大小"<<save_mid_time.size();
            qDebug()<<"数组大大小"<<save_mid_time[0].size();
            qDebug()<<"数组大大大小"<<save_mid_time[0].size();
            QVariant tp=QVariant::fromValue(save_mid_temperature);
            QVariant ts=QVariant::fromValue(save_mid_timestamp);
            QVariant sta=QVariant::fromValue(save_mid_status);
            QVariant tm=QVariant::fromValue(save_mid_time);
            emit db.saveChart(tp,ts,sta,tm,QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
            saveTime=newtime;
            QDateTime end=QDateTime::currentDateTime();
            qDebug()<<"结束"<<end.toUTC();
        }

        ULONG len=Receive(settings.devicetype,settings.deviceid,settings.canid,objs,50,100);

        qDebug()<<"收到"<<len<<"帧";
        for(uint i=0;i<len;i++){
            emit sendMessage(m_x+i,objs[i],QDateTime::currentDateTime().toString("yyyy_MM_dd hh:mm:ss:zzz"),companytypecode);
        }
        m_x+=len;
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
//    qDebug()<<settings.Bt_temperature;
//    qDebug()<<settings.Tp_temperature;
//    QMessageBox::information(this,"提示",QCoreApplication::applicationDirPath(),QMessageBox::Warning);

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
            ui->comboBox->setEnabled(false);
            saveTime=GetTickCount();
            ui->pushButton_2->setText("停止测试");
        }

    }else{
        iftest=false;
        timer->stop();
        ui->comboBox->setEnabled(true);
        ui->pushButton_2->setText("开启测试");
    }
}


void chartDisplay::on_pushButton_4_clicked()
{
    fchart->changeYScale(ui->lineEdit_2->text().toDouble(),ui->lineEdit->text().toDouble());
    schart->changeYScale(ui->lineEdit_2->text().toDouble(),ui->lineEdit->text().toDouble());
}

void chartDisplay::on_comboBox_currentIndexChanged(int index)
{
    companytypecode=ui->comboBox->currentIndex();
    changeCompanyType();
    qDebug()<<"123";
    resetAry();
    qDebug()<<"456";
    fchart->changeSplineName(companytypecode);
    schart->changeSplineName(companytypecode);
}

void chartDisplay::changeCompanyType(){
    //设置 名称
    for(int i=0;i<settings.lineNums;i++){
        pb[i]->setText(settings.splineName[companytypecode][i]);
    }
    //
}



void chartDisplay::on_radioButton_toggled(bool checked)
{
    qDebug()<<checked;
    PrecesionMode=checked;
}

void chartDisplay::on_lineEdit_3_editingFinished()
{
    TestTemperature=ui->lineEdit_3->text().toDouble();
    if(TestTemperature>=40&&TestTemperature<=375){
        offset=1.5;
    }
    if(TestTemperature>375&&TestTemperature<=800){
        offset=-0.004;
    }
}
