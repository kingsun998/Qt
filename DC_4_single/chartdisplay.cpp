#include "chartdisplay.h"
#include "ui_chartdisplay.h"
#include <direct.h>
#include <QTime>
#include <QMessageBox>
#include <dbcontroller.h>
#include <usbcanunion.h>
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
    RunTest=false;

    //时间启动器
    timer=new QTimer();
    timer->setInterval(250);
    connect(timer,&QTimer::timeout,this,&chartDisplay::handleTimeOut);

    saveInterval_miseconds=settings.saveInterval_minus*60*1000;
    //设置多选按钮
    btg=new QButtonGroup();
    //设置大小
    timestart.resize(settings.totalnums);
    records_bt.resize(settings.totalnums);
    timeend.resize(settings.totalnums);
    records_tp.resize(settings.totalnums);


    pb.resize(settings.totalnums);
    label_Val.resize(settings.totalnums);
    label_Time.resize(settings.totalnums);
    label_Status.resize(settings.totalnums);
    label_testStatus.resize(settings.totalnums);
    label_testError.resize(settings.totalnums);
    precisionStatus.resize(settings.totalnums);
    precisionError.resize(settings.totalnums);

    for(int i=0;i<settings.totalnums;i++){
        pb[i]=new QCheckBox();
        pb[i]->setText(settings.splineName[companytypecode][i]);
        vboxlayout[3].addWidget(pb[i]);
        btg->addButton(pb[i]);
        timestart[i]=0;
        timeend[i]=0;

        precisionError[i]=0;
        precisionStatus[i]=true;

        pb[i]->setCheckState(Qt::Checked);
        records_bt[i]=false;
        records_tp[i]=false;

        label_Val[i]=new QLabel();
        label_Time[i]=new QLabel();
        label_Status[i]=new QLabel();
        label_testStatus[i]=new QLabel();
        label_testError[i]=new QLabel();

        vboxlayout[0].addWidget(label_Val[i]);
        vboxlayout[1].addWidget(label_Status[i]);
        vboxlayout[2].addWidget(label_Time[i]);
        vboxlayout[4].addWidget(label_testStatus[i]);
        vboxlayout[5].addWidget(label_testError[i]);
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
    groupbox[4].setTitle("测试状态");
    groupbox[4].setMinimumWidth(100);
    groupbox[4].setLayout(&vboxlayout[4]);
    groupbox[5].setTitle("测试误差");
    groupbox[5].setMinimumWidth(100);
    groupbox[5].setLayout(&vboxlayout[5]);

    ui->horizontalLayout_2->addWidget(&groupbox[3]);
    ui->horizontalLayout_2->addWidget(&groupbox[0]);
    ui->horizontalLayout_2->addWidget(&groupbox[1]);
    ui->horizontalLayout_2->addWidget(&groupbox[2]);
    ui->horizontalLayout_2->addWidget(&groupbox[4]);
    ui->horizontalLayout_2->addWidget(&groupbox[5]);
    // 设置不互斥
    btg->setExclusive(false);//这样的话就支持多选了。
    QList<QAbstractButton *> list=btg->buttons();
    for(int i=0;i<settings.lineNums;i++){
        list[i]->setChecked(true);
    }

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
    connect(this,&chartDisplay::sendtochart_first,fchart,&Mychart::getMessage);
    connect(this,&chartDisplay::sendtochart_second,schart,&Mychart::getMessage);
    connect(&pretimer,&QTimer::timeout,this,&chartDisplay::handlePrePrecesionTimeOut);
    ui->comboBox->setMaximumWidth(100);

    QString dir=QCoreApplication::applicationDirPath()+"/savefiles/";
    QString chartdir=QCoreApplication::applicationDirPath()+"/savefiles/charts/";
    QString framedir=QCoreApplication::applicationDirPath()+"/savefiles/frames/";

    //test
    PrecesionMode=false;
    TestTemperature=0;
    offset=0;

    current_compnay=0;
    IfTestPrePrecesion=false;
    pretime=0;


    sendtimer=new QTimer();

    frameConfig={
                 {"SendType",{{"正常发送",0},{"单次发送",1}}},
                 {"RemoteFlag",{{"数据帧",0},{"远程帧",1}}},
                 {"ExternFlag",{{"标准帧",0},{"扩展帧",1}}}
                };
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

}




int chartDisplay::Calculate(int index,double y){
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
        records_tp[index]=true;
    }

    if(records_tp[index]){
        double tp=(timeend[index]-timestart[index])/1000;
        return tp;
    }else{
        return 0;
    }
}

int chartDisplay::PrecesionTest(int index,double val){
    QPalette ps;
    bool flg;
    if(precisionStatus[index]){
        if(!IfTestPrePrecesion){
            precisionError[index]=val-TestTemperature;
            if(offset>0){
                flg=abs(precisionError[index])<TestTemperature*(offset);
            }else{
                flg=abs(precisionError[index])<4;
            }
            int idx=(flg==true?1:0);
            ps.setColor(QPalette::WindowText,settings.Test_status_color[idx]);
            label_testStatus[index]->setText(settings.Test_status[idx]);
            label_testStatus[index]->setPalette(ps);
            precisionStatus[index]=flg;
            label_testError[index]->setText(QString::number(precisionError[index]));
            label_testError[index]->setPalette(ps);
            return idx;
        }
        else{
            ps.setColor(QPalette::WindowText,settings.Test_status_color[2]);
            label_testStatus[index]->setText("...");
            label_testStatus[index]->setPalette(ps);
            label_testError[index]->setText("...");
            label_testError[index]->setPalette(ps);
            return 3;
        }

    }
    return 0;
}

void chartDisplay::show_detail(long mx,CAN_OBJ obj,QString datetime,int companycode){
    int error1=0,error2=0,error3=0;
    int base=1;
    int tmp;
    double V1,V2,V3,V4;
    double Tcf,Tcs,Tct;
    int index1,index2,index3,index4;
    double tp;
    switch (obj.ID) {
    ///*
    /// A1
    ///*
    case 0x18FD2083: //0x18FD2083 A1 T1  T4
        if(current_compnay){
            return;
        }
        index1=0;
        index2=3;

        //计算数值和时间
        Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
        Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

        emit sendtochart_first(mx/4,0,Tcf,0);
        emit sendtochart_first(mx/4,3,Tcs,0);

        if(PrecesionMode){
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
            ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionError"].push_back(QString::number(precisionError[index1]));
            ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionError"].push_back(QString::number(precisionError[index2]));
        }else{
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionTest"].push_back("No test");
            ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionTest"].push_back("No test");
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionError"].push_back("No test");
            ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionError"].push_back("No test");
        }
        //保存数据
        ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["val"].push_back(QString::number(Tcf,'f',2));
        //计算时间
        tp=Calculate(index1,Tcf);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Val[index1]->setText(QString::number(Tcf,'f',2));
        label_Time[index1]->setText(QString::number(tp>0?tp:0,'f',2));

        ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["val"].push_back(QString::number(Tcs,'f',2));
        tp=Calculate(index2,Tcs);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Time[index2]->setText(QString::number(tp>0?tp:0,'f',2));
        label_Val[index2]->setText(QString::number(Tcs,'f',2));
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
        label_Status[index1]->setText(settings.errorCode_TC[error1]);
        label_Status[index2]->setText(settings.errorCode_TC[error2]);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["status"].push_back(settings.errorCode_TC[error1]);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["status"].push_back(settings.errorCode_TC[error2]);
        // 接受时间
        receivetime.push_back(datetime);

        break;
     case 0x14FD3E83:  //0x14FD3E83 A1 T2  T3
         if(current_compnay){
            return;
         }
         index1=1;
         index2=2;

         Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
         Tcs=(obj.Data[4]*256+obj.Data[3])*0.03125-273;

         emit sendtochart_first(mx/4,1,Tcf,0);
         emit sendtochart_first(mx/4,2,Tcs,0);
         if(PrecesionMode){
             ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
             ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
             ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionError"].push_back(QString::number(precisionError[index1]));
             ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionError"].push_back(QString::number(precisionError[index2]));
         }else{
             ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionTest"].push_back("No test");
             ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionTest"].push_back("No test");
             ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionError"].push_back("No test");
             ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionError"].push_back("No test");
         }
         //保存数据
         ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["val"].push_back(QString::number(Tcf,'f',2));
         //计算时间
         tp=Calculate(index1,Tcf);
         ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
         label_Time[index1]->setText(QString::number(tp>0?tp:0,'f',2));
         label_Val[index1]->setText(QString::number(Tcf,'f',2));


         ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["val"].push_back(QString::number(Tcs,'f',2));
         tp=Calculate(index2,Tcs);
         ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
         label_Time[index2]->setText(QString::number(tp>0?tp:0,'f',2));
         label_Val[index2]->setText(QString::number(Tcs,'f',2));

         //计算错误码
         for(int i=0;i<5;i++){
             tmp=obj.Data[2]>>i&1;
             error1+=base*tmp;
             base*=2;
         }
         base=1;
         for(int i=0;i<5;i++){
             tmp=obj.Data[5]>>i&1;
             error2+=base*tmp;
             base*=2;
         }

         label_Status[index1]->setText(settings.errorCode_TC[error1]);
         label_Status[index2]->setText(settings.errorCode_TC[error2]);
         ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["status"].push_back(settings.errorCode_TC[error1]);
         ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["status"].push_back(settings.errorCode_TC[error2]);
         break;
    case 0x18FDB483: //0x18FDB483  AT1IG2
        if(current_compnay){
            return;
        }
        index1=4;
        Tcf=(obj.Data[3]*256+obj.Data[2])*0.03125-273;
        emit sendtochart_second(mx/4,0,Tcf,1);
        if(PrecesionMode){
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionError"].push_back(QString::number(precisionError[index1]));
        }else{
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionTest"].push_back("No test");
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionError"].push_back("No test");
        }
        //保存数据
        ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["val"].push_back(QString::number(Tcf,'f',2));
        //计算时间
        tp=Calculate(index1,Tcf);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Time[index1]->setText(QString::number(tp>0?tp:0,'f',2));
        label_Val[index1]->setText(QString::number(Tcf,'f',2));

        //计算错误码
        for(int i=0;i<5;i++){
            tmp=obj.Data[5]>>i&1;
            error1+=base*tmp;
            base*=2;
        }

        label_Status[index1]->setText(settings.errorCode_TC[error1]);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["status"].push_back(settings.errorCode_TC[error1]);
        break;
    case 0x18FF75D3: //0x18FF75D3 A1 CJ  μC
        if(current_compnay){
            return;
        }
        index1=5;
        index2=6;
        //计算数值和时间
        Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
        Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

        emit sendtochart_second(mx/4,1,Tcf,1);
        emit sendtochart_second(mx/4,2,Tcs,1);

        if(PrecesionMode){
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
            ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionError"].push_back(QString::number(precisionError[index1]));
            ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionError"].push_back(QString::number(precisionError[index2]));
        }else{
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionTest"].push_back("No test");
            ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionTest"].push_back("No test");
            ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["precisionError"].push_back("No test");
            ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["precisionError"].push_back("No test");
        }
        //保存数据
        ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["val"].push_back(QString::number(Tcf,'f',2));
        //计算时间
        tp=Calculate(index1,Tcf);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Time[index1]->setText(QString::number(tp>0?tp:0,'f',2));
        label_Val[index1]->setText(QString::number(Tcf,'f',2));


        ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["val"].push_back(QString::number(Tcs,'f',2));
        tp=Calculate(index2,Tcs);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Time[index2]->setText(QString::number(tp>0?tp:0,'f',2));
        label_Val[index2]->setText(QString::number(Tcs,'f',2));
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
        label_Status[index1]->setText(settings.errorCode_TC[error1]);
        label_Status[index2]->setText(settings.errorCode_TC[error2]);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index1]]["status"].push_back(settings.errorCode_TC[error1]);
        ReceiveVal_A.obj[settings.company_usedate_names[0][index2]]["status"].push_back(settings.errorCode_TC[error2]);
        break;

    case 0x10FD4783:   //0x10FD4783 A  测试电压值
        if(current_compnay){
            return;
        }
        index1=7;
        index2=8;
        index3=9;
        index4=10;
        V1=(obj.Data[1]*256+obj.Data[0])*1.0/200-100;
        V2=(obj.Data[3]*256+obj.Data[2])*1.0/200-100;
        V3=(obj.Data[5]*256+obj.Data[4])*1.0/200-100;
        V4=(obj.Data[7]*256+obj.Data[6])*1.0/200-100;
        label_Val[index1]->setText(QString::number(V1,'f',2));
        label_Val[index2]->setText(QString::number(V2,'f',2));
        label_Val[index3]->setText(QString::number(V3,'f',2));
        label_Val[index4]->setText(QString::number(V4,'f',2));
        break;
    ///*
    /// B1
    /// *
    case 0x18FF76D3:  //0x18FF76D3  B1 2-1 2-2
        if(!current_compnay){
            return;
        }
        //计算数值和时间
        index1=0;
        index2=1;
        Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
        Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

        emit sendtochart_first(mx/4,0,Tcf,0);
        emit sendtochart_first(mx/4,1,Tcs,0);

        if(PrecesionMode){
            ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
            ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionError"].push_back(QString::number(precisionError[index1]));
            ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionError"].push_back(QString::number(precisionError[index2]));
        }else{
            ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionTest"].push_back("No test");
            ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionTest"].push_back("No test");
            ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionError"].push_back("No test");
            ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionError"].push_back("No test");
        }
        //保存数据
        ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["val"].push_back(QString::number(Tcf,'f',2));
        //计算时间
        tp=Calculate(index1,Tcf);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Time[index1]->setText(QString::number(tp>0?tp:0,'f',2));
        label_Val[index1]->setText(QString::number(Tcf,'f',2));

        ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["val"].push_back(QString::number(Tcs,'f',2));
        tp=Calculate(index2,Tcs);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Time[index2]->setText(QString::number(tp>0?tp:0,'f',2));
        label_Val[index2]->setText(QString::number(Tcs,'f',2));
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
        label_Status[index1]->setText(settings.errorCode_TC[error1]);
        label_Status[index2]->setText(settings.errorCode_TC[error2]);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["status"].push_back(settings.errorCode_TC[error1]);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["status"].push_back(settings.errorCode_TC[error2]);

        receivetime.push_back(datetime);
        break;
     case 0x18FF77D3:  //0x18FF77D3 B1 CJ  μC
        if(!current_compnay){
            return;
        }
         index1=2;
         index2=3;
         Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
         Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

         emit sendtochart_first(mx/4,2,Tcf,0);
         emit sendtochart_first(mx/4,3,Tcs,0);
         if(PrecesionMode){
             ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
             ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
             ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionError"].push_back(QString::number(precisionError[index1]));
             ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionError"].push_back(QString::number(precisionError[index2]));
         }else{
             ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionTest"].push_back("No test");
             ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionTest"].push_back("No test");
             ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionError"].push_back("No test");
             ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionError"].push_back("No test");
         }
         //保存数据
         ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["val"].push_back(QString::number(Tcf,'f',2));
         //计算时间
         tp=Calculate(index1,Tcf);
         ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
         label_Time[index1]->setText(QString::number(tp>0?tp:0,'f',2));
         label_Val[index1]->setText(QString::number(Tcf,'f',2));

         ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["val"].push_back(QString::number(Tcs,'f',2));
         tp=Calculate(index2,Tcs);
         ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
         label_Time[index2]->setText(QString::number(tp>0?tp:0,'f',2));
         label_Val[index2]->setText(QString::number(Tcs,'f',2));

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
         label_Status[index1]->setText(settings.errorCode_TC[error1]);
         label_Status[index2]->setText(settings.errorCode_TC[error2]);
         ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["status"].push_back(settings.errorCode_TC[error1]);
         ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["status"].push_back(settings.errorCode_TC[error2]);
         break;

    case 0x18FF74D3: //0x18FF7483  B1 3-1 3-2 3-3
        if(!current_compnay){
            return;
        }
        index1=4;
        index2=5;
        index3=6;
        Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
        Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;
        Tct=(obj.Data[5]*256+obj.Data[4])*0.03125-273;

        emit sendtochart_second(mx/4,0,Tcf,1);
        emit sendtochart_second(mx/4,1,Tcs,1);
        emit sendtochart_second(mx/4,2,Tct,1);

        if(PrecesionMode){
            ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
            ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            ReceiveVal_B.obj[settings.company_usedate_names[1][index3]]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index3,Tcs)]);
            ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionError"].push_back(QString::number(precisionError[index1]));
            ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionError"].push_back(QString::number(precisionError[index2]));
            ReceiveVal_B.obj[settings.company_usedate_names[1][index3]]["precisionError"].push_back(QString::number(precisionError[index3]));
        }else{
            ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionTest"].push_back("No test");
            ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionTest"].push_back("No test");
            ReceiveVal_B.obj[settings.company_usedate_names[1][index3]]["precisionTest"].push_back("No test");
            ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["precisionError"].push_back("No test");
            ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["precisionError"].push_back("No test");
            ReceiveVal_B.obj[settings.company_usedate_names[1][index3]]["precisionError"].push_back("No test");
        }


        ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["val"].push_back(QString::number(Tcf,'f',2));
        tp=Calculate(index1,Tcf);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Time[index1]->setText(QString::number(tp>0?tp:0,'f',2));
        label_Val[index1]->setText(QString::number(Tcf,'f',2));

        ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["val"].push_back(QString::number(Tcs,'f',2));
        tp=Calculate(index2,Tcs);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Time[index2]->setText(QString::number(tp>0?tp:0,'f',2));
        label_Val[index2]->setText(QString::number(Tcs,'f',2));

        ReceiveVal_B.obj[settings.company_usedate_names[1][index3]]["val"].push_back(QString::number(Tct,'f',2));
        tp=Calculate(index3,Tct);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index3]]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
        label_Time[index3]->setText(QString::number(tp>0?tp:0,'f',2));
        label_Val[index3]->setText(QString::number(Tct,'f',2));

        base=1;
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
        base=1;
        for(int i=2;i<7;i++){
            tmp=obj.Data[7]>>i&1;
            error3+=base*tmp;
            base*=2;
        }
        label_Status[index1]->setText(settings.errorCode_TC[error1]);
        label_Status[index2]->setText(settings.errorCode_TC[error2]);
        label_Status[index3]->setText(settings.errorCode_TC[error3]);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index1]]["status"].push_back(settings.errorCode_TC[error1]);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index2]]["status"].push_back(settings.errorCode_TC[error2]);
        ReceiveVal_B.obj[settings.company_usedate_names[1][index3]]["status"].push_back(settings.errorCode_TC[error3]);
        break;

    case 0x10FD47D3:   //0x10FD47D3   //测试用数据帧，热电偶电压值
        if(!current_compnay){
            return;
        }
        index1=7;
        index2=8;
        index3=9;
        index4=10;
        V1=(obj.Data[1]*256+obj.Data[0])*1.0/200-100;
        V2=(obj.Data[3]*256+obj.Data[2])*1.0/200-100;
        V3=(obj.Data[5]*256+obj.Data[4])*1.0/200-100;
        V4=(obj.Data[7]*256+obj.Data[6])*1.0/200-100;
        label_Val[index1]->setText(QString::number(V1,'f',2));
        label_Val[index2]->setText(QString::number(V2,'f',2));
        label_Val[index3]->setText(QString::number(V3,'f',2));
        label_Val[index4]->setText(QString::number(V4,'f',2));
         break;
    }

}
void chartDisplay::getPrePrecesionTime(){
    this->pretime=ui->lineEdit_4->text().toInt();
    pretimer.setInterval(pretime*1000);
    int size=this->precisionStatus.size();
    for (int i=0;i<size;i++) {
        precisionStatus[i]=true;
    }
    IfTestPrePrecesion=true;
    pretimer.start();
}
void chartDisplay::handlePrePrecesionTimeOut(){
    IfTestPrePrecesion=false;
    pretimer.stop();
}

Q_DECLARE_METATYPE(QVector<QVector<QString>>)
Q_DECLARE_METATYPE(QVector<QString>)
Q_DECLARE_METATYPE(QVector<QVector<double>>)
void chartDisplay::handleTimeOut(){
        //这部分是公用的
        double newtime=GetTickCount();
        if(newtime-saveTime>saveInterval_miseconds){
            QDateTime start=QDateTime::currentDateTime();
            int index=ui->comboBox->currentIndex();
            //swap 速度快，且清空

            if(index==0){
                dbSingleChart *obj=new dbSingleChart(ReceiveVal_A.obj,
                                                     receivetime,
                                                     index);
                ReceiveVal_A.reset();
                dbcontroller.addObject(obj);
            }else if(index==1) {

                dbSingleChart *obj=new dbSingleChart(ReceiveVal_B.obj,
                                                     receivetime,
                                                     index);
                ReceiveVal_B.reset();
                dbcontroller.addObject(obj);
            }
            saveTime=newtime;
            QDateTime end=QDateTime::currentDateTime();
        }

        if(settings.TestMode){
            ULONG len=usbcanunion.StartTestMode();
            for(uint i=0;i<len;i++){
                emit sendMessage(m_x+i,usbcanunion.objs[i],QDateTime::currentDateTime().toString("yyyy_MM_dd hh:mm:ss:zzz"),companytypecode);
            }
            m_x+=len;
        }
        else{
            ULONG len=usbcanunion.ReceiveFromCan();
            for(uint i=0;i<len;i++){
                emit sendMessage(m_x+i,usbcanunion.objs[i],QDateTime::currentDateTime().toString("yyyy_MM_dd hh:mm:ss:zzz"),companytypecode);
            }
            m_x+=len;

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
        settings.caninit=true;
    }

}

//关闭硬件
void chartDisplay::on_pushButton_3_clicked()
{
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

//控制测试的开始和暂停
void chartDisplay::on_pushButton_2_clicked(bool checked)
{
        if(settings.TestMode){
            if(RunTest==false){
                //    测试代码
                RunTest=true;
                emit usbcanunion.setFrameStartTime();
                timer->start(100);
                ui->comboBox->setEnabled(false);
                saveTime=GetTickCount();
                ui->pushButton_2->setText("停止测试");
            }
            else{
                RunTest=false;
                timer->stop();
                ui->comboBox->setEnabled(true);
                ui->pushButton_2->setText("开启测试");
            }
        }
        else{
            if(RunTest==false){
                DWORD drl;
                drl=usbcanunion.startUsbCan();
                if (drl != STATUS_OK)
                {
                    QMessageBox::information(this,"提示","开启失败",QMessageBox::Warning);
                    return ;
                }else{
                    RunTest=true;
                    emit usbcanunion.setFrameStartTime();
                    timer->start(100);
                    //重新技术
                    ui->comboBox->setEnabled(false);
                    saveTime=GetTickCount();
                    ui->pushButton_2->setText("停止测试");
                }
            }else{
                RunTest=false;
                timer->stop();
                ui->comboBox->setEnabled(true);
                ui->pushButton_2->setText("开启测试");
            }
        }
}

//两个动态表的尺度更改
void chartDisplay::on_pushButton_4_clicked()
{
    fchart->changeYScale(ui->lineEdit_2->text().toDouble(),ui->lineEdit->text().toDouble());
    schart->changeYScale(ui->lineEdit_2->text().toDouble(),ui->lineEdit->text().toDouble());
}

void chartDisplay::on_comboBox_currentIndexChanged(int index)
{
    settings.CompanyType=ui->comboBox->currentIndex();
    current_compnay=ui->comboBox->currentIndex();
    companytypecode=ui->comboBox->currentIndex();
    changeCompanyType();
    resetAry();
    fchart->changeSplineName(companytypecode);
    schart->changeSplineName(companytypecode);
}

void chartDisplay::changeCompanyType(){
    //设置 名称
    for(int i=0;i<settings.lineNums;i++){
        pb[i]->setText(settings.splineName[companytypecode][i]);
    }
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
// 发送按钮
void chartDisplay::on_pushButton_6_clicked()
{
     if(ui->radioButton_3->isChecked()){
         sendtimes=ui->lineEdit_10->text().toUInt();
         sendtimer->setInterval(ui->spinBox_2->text().toInt());
         sendtimer->start();
     }
     else{
         sendSingleFrameToCan();
     }
}

//发送单帧
void chartDisplay::sendSingleFrameToCan()
{
    CAN_OBJ obj;
    QStringList slist=ui->lineEdit_9->text().split(" ");
    for(int i=0;i<slist.size();i++){
        obj.Data[i]=slist[i].toUInt(nullptr,16);
    }

    obj.ID=ui->lineEdit_8->text().toUInt(nullptr,16);

    obj.SendType=frameConfig["SendType"][ui->comboBox_3->currentText()];
    obj.RemoteFlag=frameConfig["RemoteFlag"][ui->comboBox_2->currentText()];
    obj.ExternFlag=frameConfig["ExternFlag"][ui->comboBox_4->currentText()];
    obj.DataLen=8;
    if(usbcanunion.SendToCan(obj)){
         emit sendsingleframetocan(-1,obj,QDateTime::currentDateTime().toString("yyyy_MM_dd hh:mm:ss:zzz"),-1);
    }
}

// 发送连续帧
void chartDisplay::handleSenderTimeOut(){
    if(sendtimes==0){
        sendtimer->stop();
        return;
    }
    sendSingleFrameToCan();
    sendtimes-=1;
}
// 是否连续发送
void chartDisplay::on_radioButton_3_clicked(bool checked)
{
    qDebug()<<checked;
    ui->lineEdit_10->setEnabled(!checked);
    ui->spinBox_2->setEnabled(!checked);
}


void chartDisplay::on_checkBox_stateChanged(int arg1)
{
    qDebug()<<"checked"<<arg1;
    PrecesionMode=arg1;
    if(arg1){
        QString &&tmp=ui->lineEdit_3->text();
        TestTemperature=tmp.simplified()==""?20:tmp.toDouble();
        if(TestTemperature<500){
            offset=-4;
        }
        else{
            offset=0.008;
        }
        tmp=ui->lineEdit_4->text();
        this->pretime=tmp.simplified()==""?0:tmp.toInt();
        IfTestPrePrecesion=true;
        pretimer.setInterval(this->pretime*1000);
        pretimer.start();
    }
    int size=this->precisionStatus.size();
    for (int i=0;i<size;i++) {
        precisionStatus[i]=true;
    }
}
