#include "testdisplay.h"
#include "ui_testdisplay.h"
#include "QDateTime"
#include <usbcanunion.h>
#include <dbcontroller.h>
#include "QMessageBox"

TestDisplay::TestDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestDisplay)
{
    ui->setupUi(this);
    int index=0;
    QWidget * w=nullptr;
    //使用的名称
    company_usedate=settings.company_usedate_names;
    company_date=settings.company_datenames;
    vboxlayout.append(new QVBoxLayout());
    gridlayout.append(new QGridLayout());
//    QFont font;
//    font.setPixelSize(5);
    for(int i=0;i<2;i++){
        index=i;
        mycharts.append(new Mychart(0,0,0,3,0));
        mycharts.append(new Mychart(0,0,1,3,0));

        chartviews.append(new QChartView(mycharts[2*i]));
        chartviews.append(new QChartView(mycharts[2*i+1]));
//        mycharts[2*i]->setFont(font);
//        mycharts[2*i+1]->setFont(font);
        // 设置了宽为2
        gridlayout[0]->addWidget(chartviews[2*i],0,i);
        gridlayout[0]->addWidget(chartviews[2*i+1],1,i);
    }

    vboxlayout[0]->addLayout(gridlayout[0]);
    //
    groupboxs.append(new QGroupBox());
    groupboxs[0]->setTitle("曲线");

    buttongroup=new QButtonGroup();
    gridlayout.append(new QGridLayout());
    gridlayout[1]->addWidget(new QCheckBox("型号"),0,0);
    for(int j=0;j<company_date.size();j++){
        checkboxs.append(new QCheckBox(company_date[j]));
        buttongroup->addButton(checkboxs[j]);
        gridlayout[1]->addWidget(checkboxs[j],j+1,0);
    }
    groupboxs[0]->setLayout(gridlayout[1]);

    //控制区域
    gridlayout.append(new QGridLayout());

    QVector<QString> buttonnames={"跳转","扫描二维码","发送数据","连接设备","开启设备","关闭设备"};
    QVector<QString> lineeditnames={"Y轴最小值","Y轴最大值","输入稳定性阈值","输入要测试的精度","输入预测试时间"};
    QStringList radionames={"高温稳定性测试","精度测试"};

    buttonmap={{"scalejump",0},{"scan",1},{"senddata",2},{"connectdevice",3},{"opendevice",4},{"closedevice",5}};
    lineeditmap={{"xaxis",0},{"yaxis",1},{"hightemp",2},{"precesionval",3},{"precesiontime",4}};

    for (int i=0;i<radionames.size();i++) {
        lineedit.append(new QLineEdit());
        lineedit[i]->setPlaceholderText(lineeditnames[i]);
        gridlayout[2]->addWidget(lineedit[i],0,i,1,1);
    }

    QSize buttonsize(150,45);
    pushbuttons.append(new QPushButton(buttonnames[buttonmap["scalejump"]]));
    pushbuttons[0]->setMinimumSize(buttonsize);
    gridlayout[2]->addWidget(pushbuttons[buttonmap["scalejump"]],0,2);

    pushbuttons.append(new QPushButton(buttonnames[buttonmap["scan"]]));
    pushbuttons[1]->setMinimumSize(buttonsize);
    gridlayout[2]->addWidget(pushbuttons[buttonmap["scan"]],0,3);

    pushbuttons.append(new QPushButton(buttonnames[buttonmap["senddata"]]));
    pushbuttons[2]->setMinimumSize(buttonsize);
    gridlayout[2]->addWidget(pushbuttons[buttonmap["senddata"]],0,4);

    vboxlayout[0]->addLayout(gridlayout[2]);

    gridlayout.append(new QGridLayout());
    combobox.append(new QComboBox());
    combobox[0]->addItems(settings.CompanyName);
    combobox[0]->setMinimumSize(QSize(50,40));
    combobox[0]->setMinimumSize(buttonsize);
    gridlayout[3]->addWidget(combobox[0],0,0);


    for (int i=0;i<3;i++) {
        pushbuttons.append(new QPushButton(buttonnames[i+buttonmap["connectdevice"]]));
        gridlayout[3]->addWidget(pushbuttons[i+buttonmap["connectdevice"]],0,i+1);
        pushbuttons[3+i]->setMinimumSize(buttonsize);
    }
    testname={"高温稳定性测试","精度测试"};
    highTempCheckBox=new QCheckBox(testname[0]);
    gridlayout[3]->addWidget(highTempCheckBox,0,4);

    lineedit.append(new QLineEdit());
    lineedit[lineeditmap["hightemp"]]->setPlaceholderText(lineeditnames[lineeditmap["hightemp"]]);
    lineedit[lineeditmap["hightemp"]]->setMaximumWidth(200);
    gridlayout[3]->addWidget(lineedit[lineeditmap["hightemp"]],0,5);

    precisionCheckBox=new QCheckBox(testname[1]);
    gridlayout[3]->addWidget(precisionCheckBox,0,6);

    lineedit.append(new QLineEdit());
    lineedit[lineeditmap["precesionval"]]->setPlaceholderText(lineeditnames[lineeditmap["precesionval"]]);
    lineedit[lineeditmap["precesionval"]]->setMaximumWidth(200);
    gridlayout[3]->addWidget(lineedit[lineeditmap["precesionval"]],0,7);

    lineedit.append(new QLineEdit());
    lineedit[lineeditmap["precesiontime"]]->setPlaceholderText(lineeditnames[lineeditmap["precesiontime"]]);
    lineedit[lineeditmap["precesiontime"]]->setMaximumWidth(200);
    gridlayout[3]->addWidget(lineedit[lineeditmap["precesiontime"]],0,8);

    vboxlayout[0]->addLayout(gridlayout[3]);

    //添加测试按钮 checkebox
    testcheckbox.push_back(highTempCheckBox);
    testcheckbox.push_back(precisionCheckBox);

    w=new QWidget();
    w->setLayout(vboxlayout[0]);
    ui->horizontalLayout->addWidget(w);
    //这是上一个的添加
    ui->horizontalLayout->addWidget(groupboxs[0]);

    QVector<QString> colnames={"数值","传感器状态","时间","测试状态"};
    for (int i=0;i<colnames.size();i++) {

        groupboxs2.append(new QGroupBox());
        groupboxs2[i]->setTitle(colnames[i]);
        gridlayout2.append(new QGridLayout());

        gridlayout2[i]->addWidget(new QLabel("A1"),0,0);
        gridlayout2[i]->addWidget(new QLabel("A2"),0,1);

        QVector<QLabel *> vec;
        showLabel.append(vec);
        for(int j=0;j<company_date.size();j++){
            showLabel[i].append(new QLabel());
            showLabel[i].append(new QLabel());
            gridlayout2[i]->addWidget(showLabel[i][2*j],j+1,0);
            gridlayout2[i]->addWidget(showLabel[i][2*j+1],j+1,1);
        }
        groupboxs2[i]->setLayout(gridlayout2[i]);
        ui->horizontalLayout->addWidget(groupboxs2[i]);
    }

    //初始化高温稳定性测试数组
    highTempTest.resize(2*company_date.size());
    highTempStatus.resize(2*company_date.size());
    precisionStatus.resize(2*company_date.size());
    precisionError.resize(2*company_date.size());
    for (int i=0;i<2*company_date.size();i++) {
        highTempStatus[i]=true;
        highTempTest[i]=0;
        precisionStatus[i]=true;
    }
    //低通滤波
    recordVal.resize(2*company_date.size());
    recordValTemp.resize(2*company_date.size());
    show_clock.resize(2*company_date.size());
    //互斥
    buttongroup->setExclusive(false);
    //设置为true
    for(int i=0;i<company_usedate.size();i++){
        checkboxs[i]->setChecked(true);
    }
    for (int i=company_usedate.size();i<company_date.size();i++) {
        checkboxs[i]->setEnabled(false);
    }
    //时间启动器
    timer=new QTimer();
    timer->setInterval(250);
    //连接
    connect(buttongroup,SIGNAL(buttonToggled(int,bool)),this,SLOT(shspline(int,bool)));
    connect(timer,&QTimer::timeout,this,&TestDisplay::handleTimeOut);
    connect(this,&TestDisplay::sendMessage,this,&TestDisplay::show_detail);

    connect(this,&TestDisplay::sendtochart_First_Up,mycharts[0],&Mychart::getMessage);
    connect(this,&TestDisplay::sendtochart_First_Down,mycharts[1],&Mychart::getMessage);
    connect(this,&TestDisplay::sendtochart_Second_Up,mycharts[2],&Mychart::getMessage);
    connect(this,&TestDisplay::sendtochart_Second_Down,mycharts[3],&Mychart::getMessage);

    //  currentIndexChanged 有两个重载，一个(int)  (qstring)
    connect(combobox[0],QOverload<int>::of(&QComboBox::currentIndexChanged),this,&TestDisplay::change_Company);

    connect(highTempCheckBox,&QCheckBox::clicked,this,&TestDisplay::highTempTestSwitch);
    connect(precisionCheckBox,&QCheckBox::clicked,this,&TestDisplay::precisionTestSwitch);

    //跳转按钮
    connect(pushbuttons[buttonmap["scalejump"]],&QPushButton::clicked,this,&TestDisplay::YscaleJump);
    // Can操作
    connect(pushbuttons[buttonmap["scan"]],&QPushButton::clicked,this,&TestDisplay::newscan);
    connect(pushbuttons[buttonmap["senddata"]],&QPushButton::clicked,this,&TestDisplay::senddata);
    connect(pushbuttons[buttonmap["connectdevice"]],&QPushButton::clicked,this,&TestDisplay::connectCan);
    connect(pushbuttons[buttonmap["opendevice"]],&QPushButton::clicked,this,&TestDisplay::startCan);
    connect(pushbuttons[buttonmap["closedevice"]],&QPushButton::clicked,this,&TestDisplay::disconnectCan);
    //测试
    connect(lineedit[lineeditmap["hightemp"]],&QLineEdit::editingFinished,this,&TestDisplay::getHighThreshold);
//    connect(lineedit[lineeditmap["precesionval"]],&QLineEdit::editingFinished,this,&TestDisplay::getPrecesion);
//    connect(lineedit[lineeditmap["precesiontime"]],&QLineEdit::editingFinished,this,&TestDisplay::getPrePrecesionTime);
    connect(&pretimer,&QTimer::timeout,this,&TestDisplay::handlePrePrecesionTimeOut);
    saveInterval_miseconds=settings.saveInterval_minus*60*1000;

    testMode=0;
    //这是index
    m_x=0;

    ReceiveVal_A1.setParameter("A");
    ReceiveVal_A2.setParameter("A");
    ReceiveVal_B1.setParameter("B");
    ReceiveVal_B2.setParameter("B");

    ReceiveTimestamp.resize(2);

    timestart.resize(2*company_usedate.size());
    records_bt.resize(2*company_usedate.size());
    timeend.resize(2*company_usedate.size());
    records_tp.resize(2*company_usedate.size());


    //初始是没有开始运行的
    IfRun=false;
    IfTestHighTemp=false;
    IfTestPrecision=false;
    IfTestPrePrecesion=false;
//    lineedit[lineeditmap["precesiontime"]]->setEnabled(false);
    lineedit[lineeditmap["hightemp"]]->setEnabled(false);
    pretime=10;
    companyname=settings.CompanyName[0];

    offset=0;

    company_type=0;

    //更改字号
    for (int i=0;i<pushbuttons.size();i++) {
        pushbuttons[i]->setFont(*settings.pushbutton_font);
        pushbuttons[i]->setMinimumSize(100,40);
    }
    for (int i=0;i<combobox.size();i++) {
        combobox[i]->setMinimumSize(100,40);
        combobox[i]->setFont(*settings.pushbutton_font);
    }


}

TestDisplay::~TestDisplay()
{
    delete ui;
}

double TestDisplay::Calculate(int index,double y){
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

int TestDisplay::PrecesionTest(int index,double val){
    QPalette ps;
    bool flg;
    if(precisionStatus[index]){
        if(!IfTestPrePrecesion){
            val=lowPassFilter(index,val);
            precisionError[index]=val-TestTemperature;
            if(offset>0){
                flg=abs(precisionError[index])<TestTemperature*(offset);
            }else{
                flg=abs(precisionError[index])<abs(offset);
            }
            int idx=(flg==true?1:0);
            ps.setColor(QPalette::WindowText,settings.Test_status_color[idx]);
            showLabel[3][index]->setText(settings.Test_status[idx]);
            showLabel[3][index]->setPalette(ps);
            showLabel[3][index]->setFont(*settings.showlabel_font);
            precisionStatus[index]=flg;
            return idx;
        }
        else{
            ps.setColor(QPalette::WindowText,settings.Test_status_color[2]);
            showLabel[3][index]->setText("...");
            showLabel[3][index]->setPalette(ps);
            return 3;
        }

    }
    return 0;
}

int TestDisplay::highTempStableTest(int index,double val){
    //若已经为false，就直接返回false
    if(highTempStatus[index]){
        val=lowPassFilter(index,val);
        if(val<870){
            QPalette ps;
            int idx;
            if(val-highTempTest[index]<-settings.HightempThreshold){
                idx=0;
            }else{
                idx=1;
                highTempTest[index]=val;
            }
            showLabel[3][index]->setText(settings.Test_status[idx]);
            ps.setColor(QPalette::WindowText,settings.Test_status_color[idx]);
            showLabel[3][index]->setPalette(ps);
            showLabel[3][index]->setFont(*settings.showlabel_font);
            highTempStatus[index]=idx;
            return idx;
        }else{
            return 1;
        }
    }
    return 0;
}

void TestDisplay::setnotest(int index){
    QPalette ps;
    ps.setColor(QPalette::WindowText,settings.Test_status_color[2]);
    showLabel[3][index]->setText(settings.Test_status[2]);
    showLabel[3][index]->setPalette(ps);
}

void TestDisplay::sensorState(int index,std::map<int,QString>& map,int val){
     QPalette ps;
     if(val==0){
        ps.setColor(QPalette::WindowText,settings.Test_status_color[1]);
     }else{
        ps.setColor(QPalette::WindowText,settings.Test_status_color[0]);
     }
     showLabel[1][index]->setText(map[val]);
     showLabel[1][index]->setPalette(ps);
}

//更改模式，设置显示间隔
double TestDisplay::lowPassFilter(int index,double val){
    recordVal[index]=0.5*(recordVal[index]+val);
    show_clock[index]=(++show_clock[index])%settings.show_clock;
    recordValTemp[index]=show_clock[index]==0?recordVal[index]:recordValTemp[index];
    return recordValTemp[index];
}

void TestDisplay::show_detail(uint mx,CAN_OBJ obj,QString datetime){
    int error1=0;
    int error2=0;
    int base=1;
    int tmp;
    double tp;
    double V1,V2,V3,V4;
    double Tcf,Tcs;
    int index1,index2,index3,index4;
    //发送到表格
    emit SendToFrame(mx,obj,datetime,companyname);
//    这里使用id偏移适配两个种类
    switch (obj.ID) {
        ///*
        /// A1
        ///*
        case 0x18FD2083: //0x18FD2083 A1 T1  T4
            if(company_type){
                return;
            }
            index1=2*0;
            index2=2*3;
            //计算数值和时间
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

            if(IfTestHighTemp==true){
                ReceiveVal_A1.obj["T1"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index1,Tcf)]);
                ReceiveVal_A1.obj["T4"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index2,Tcs)]);
            }else{
                ReceiveVal_A1.obj["T1"]["hightempTest"].push_back(settings.Test_status[2]);
                ReceiveVal_A1.obj["T4"]["hightempTest"].push_back(settings.Test_status[2]);
            }
            if(IfTestPrecision==true){
                ReceiveVal_A1.obj["T1"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
                ReceiveVal_A1.obj["T4"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            }else{
                ReceiveVal_A1.obj["T1"]["precisionTest"].push_back(settings.Test_status[2]);
                ReceiveVal_A1.obj["T4"]["precisionTest"].push_back(settings.Test_status[2]);
            }
            if(!IfTestHighTemp&&!IfTestPrecision){
                setnotest(index1);
                setnotest(index2);
            }
            emit sendtochart_First_Up(mx/4,0,Tcf);
            emit sendtochart_First_Down(mx/4,0,Tcs);
            //保存数据
            ReceiveVal_A1.obj["T1"]["val"].push_back(QString::number(Tcf,'f',2));
            //计算时间
            tp=Calculate(index1,Tcf);
            ReceiveVal_A1.obj["T1"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            showLabel[0][index1]->setFont(*settings.showlabel_font);

            ReceiveVal_A1.obj["T4"]["val"].push_back(QString::number(Tcs,'f',2));
            tp=Calculate(index2,Tcs);
            ReceiveVal_A1.obj["T4"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            showLabel[0][index2]->setFont(*settings.showlabel_font);
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
            sensorState(index1,settings.errorCode_TC,error1);
            sensorState(index2,settings.errorCode_TC,error2);
            ReceiveVal_A1.obj["T1"]["status"].push_back(settings.errorCode_TC[error1]);
            ReceiveVal_A1.obj["T4"]["status"].push_back(settings.errorCode_TC[error2]);
            // 接受时间
            ReceiveTimestamp[0].push_back(datetime);

            break;
         case 0x14FD3E83:  //0x14FD3E83 A1 T2  T3
             if(company_type){
                 return;
             }
             index1=2*1;
             index2=2*2;
             Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
             Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

             if(IfTestHighTemp==true){
                 ReceiveVal_A1.obj["T2"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index1,Tcf)]);
                 ReceiveVal_A1.obj["T3"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index2,Tcs)]);
             }else{
                 ReceiveVal_A1.obj["T2"]["hightempTest"].push_back(settings.Test_status[2]);
                 ReceiveVal_A1.obj["T3"]["hightempTest"].push_back(settings.Test_status[2]);
             }
             if(IfTestPrecision==true){
                 ReceiveVal_A1.obj["T2"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
                 ReceiveVal_A1.obj["T3"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
             }else{
                 ReceiveVal_A1.obj["T2"]["precisionTest"].push_back(settings.Test_status[2]);
                 ReceiveVal_A1.obj["T3"]["precisionTest"].push_back(settings.Test_status[2]);
             }
             if(!IfTestHighTemp&&!IfTestPrecision){
                 setnotest(index1);
                 setnotest(index2);
             }
             emit sendtochart_First_Up(mx/4,1,Tcf);
             emit sendtochart_First_Up(mx/4,2,Tcs);
             //保存数据
             ReceiveVal_A1.obj["T2"]["val"].push_back(QString::number(Tcf,'f',2));
             //计算时间
             tp=Calculate(index1,Tcf);
             ReceiveVal_A1.obj["T2"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
             showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
             showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
             showLabel[0][index1]->setFont(*settings.showlabel_font);

             ReceiveVal_A1.obj["T3"]["val"].push_back(QString::number(Tcs,'f',2));
             tp=Calculate(index2,Tcs);
             ReceiveVal_A1.obj["T3"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
             showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
             showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
             showLabel[0][index2]->setFont(*settings.showlabel_font);
             //计算错误码
             for(int i=0;i<5;i++){
                 tmp=obj.Data[6]>>i&1;
                 error1+=base*tmp;
                 base*=2;
             }
             sensorState(2*2,settings.errorCode_TC,error1);
             ReceiveVal_A1.obj["T2"]["status"].push_back(settings.errorCode_TC[error1]);

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
             sensorState(2*3,settings.errorCode_TC,error2);
             ReceiveVal_A1.obj["T3"]["status"].push_back(settings.errorCode_TC[error2]);
             break;
        case 0x18FF7AD3: //0x18FF7AD3 A1 CJ  μC
            if(company_type){
                return;
            }
            index1=2*4;
            index2=2*5;
            //计算数值和时间
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;


            ReceiveVal_A1.obj["CJ"]["hightempTest"].push_back(".");
            ReceiveVal_A1.obj["μC"]["precisionTest"].push_back(".");

            emit sendtochart_First_Down(mx/4,1,Tcf);
            emit sendtochart_First_Down(mx/4,2,Tcs);
            //保存数据
            ReceiveVal_A1.obj["CJ"]["val"].push_back(QString::number(Tcf,'f',2));
            //计算时间
            tp=Calculate(index1,Tcf);
            ReceiveVal_A1.obj["CJ"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            showLabel[0][index1]->setFont(*settings.showlabel_font);

            ReceiveVal_A1.obj["μC"]["val"].push_back(QString::number(Tcs,'f',2));
            tp=Calculate(index2,Tcs);
            ReceiveVal_A1.obj["μC"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            showLabel[0][index2]->setFont(*settings.showlabel_font);
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
            sensorState(2*5,settings.errorCode_CJ,error1);
            sensorState(2*6,settings.errorCode_ECU,error2);
            ReceiveVal_A1.obj["CJ"]["status"].push_back(settings.errorCode_CJ[error1]);
            ReceiveVal_A1.obj["μC"]["status"].push_back(settings.errorCode_ECU[error2]);
            break;

        case 0x10FD4783:   //0x10FD4783 A1  测试电压值
            if(company_type){
                return;
            }
            index1=2*6;
            index2=2*7;
            index3=2*8;
            index4=2*9;
            V1=(obj.Data[1]*256+obj.Data[0])*1.0/200-100;
            V2=(obj.Data[3]*256+obj.Data[2])*1.0/200-100;
            V3=(obj.Data[5]*256+obj.Data[4])*1.0/200-100;
            V4=(obj.Data[7]*256+obj.Data[6])*1.0/200-100;
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,V1),'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,V2),'f',2));
            showLabel[0][index3]->setText(QString::number(lowPassFilter(index3,V3),'f',2));
            showLabel[0][index4]->setText(QString::number(lowPassFilter(index4,V4),'f',2));
            break;

        ///*
        /// A2
        ///*
        case 0x18FD2084: //0x18FD2083 A2 T1  T4
            if(company_type){
                return;
            }
            //计算数值和时间
            index1=2*0+1;
            index2=2*3+1;
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

            if(IfTestHighTemp==true){
                ReceiveVal_A2.obj["T1"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index1,Tcf)]);
                ReceiveVal_A2.obj["T4"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index2,Tcs)]);
            }else{
                ReceiveVal_A2.obj["T1"]["hightempTest"].push_back(settings.Test_status[2]);
                ReceiveVal_A2.obj["T4"]["hightempTest"].push_back(settings.Test_status[2]);
            }
            if(IfTestPrecision==true){
                ReceiveVal_A2.obj["T1"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
                ReceiveVal_A2.obj["T4"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            }else{
                ReceiveVal_A2.obj["T1"]["precisionTest"].push_back(settings.Test_status[2]);
                ReceiveVal_A2.obj["T4"]["precisionTest"].push_back(settings.Test_status[2]);
            }
            if(!IfTestHighTemp&&!IfTestPrecision){
                setnotest(index1);
                setnotest(index2);
            }

            emit sendtochart_Second_Up(mx/4,0,Tcf);
            emit sendtochart_Second_Down(mx/4,0,Tcs);
            //保存数据
            ReceiveVal_A2.obj["T1"]["val"].push_back(QString::number(Tcf,'f',2));
            //计算时间
            tp=Calculate(index1,Tcf);
            ReceiveVal_A2.obj["T1"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            showLabel[0][index1]->setFont(*settings.showlabel_font);

            ReceiveVal_A2.obj["T4"]["val"].push_back(QString::number(Tcs,'f',2));
            tp=Calculate(index2,Tcs);
            ReceiveVal_A2.obj["T4"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            showLabel[0][index2]->setFont(*settings.showlabel_font);
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
            sensorState(index1,settings.errorCode_TC,error1);
            sensorState(index2,settings.errorCode_TC,error2);

            ReceiveVal_A2.obj["T1"]["status"].push_back(settings.errorCode_TC[error1]);
            ReceiveVal_A2.obj["T4"]["status"].push_back(settings.errorCode_TC[error2]);

            ReceiveTimestamp[1].push_back(datetime);
            break;
         case 0x14FD3E84:  //0x14FD3E83 A2 T2  T3
             if(company_type){
                 return;
             }
             index1=2*1+1;
             index2=2*2+1;
             Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
             Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

             if(IfTestHighTemp==true){
                 ReceiveVal_A2.obj["T2"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index1,Tcf)]);
                 ReceiveVal_A2.obj["T3"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index2,Tcs)]);
             }else{
                 ReceiveVal_A2.obj["T2"]["hightempTest"].push_back(settings.Test_status[2]);
                 ReceiveVal_A2.obj["T3"]["hightempTest"].push_back(settings.Test_status[2]);
             }
             if(IfTestPrecision==true){
                 ReceiveVal_A2.obj["T2"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
                 ReceiveVal_A2.obj["T3"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
             }else{
                 ReceiveVal_A2.obj["T2"]["precisionTest"].push_back(settings.Test_status[2]);
                 ReceiveVal_A2.obj["T3"]["precisionTest"].push_back(settings.Test_status[2]);
             }
             if(!IfTestHighTemp&&!IfTestPrecision){
                 setnotest(index1);
                 setnotest(index2);
             }

             emit sendtochart_Second_Up(mx/4,1,Tcf);
             emit sendtochart_Second_Up(mx/4,2,Tcs);
             //保存数据
             ReceiveVal_A2.obj["T2"]["val"].push_back(QString::number(Tcf,'f',2));
             //计算时间
             tp=Calculate(index1,Tcf);
             ReceiveVal_A2.obj["T2"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
             showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
             showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
             showLabel[0][index1]->setFont(*settings.showlabel_font);

             ReceiveVal_A2.obj["T3"]["val"].push_back(QString::number(Tcs,'f',2));
             tp=Calculate(index2,Tcs);
             ReceiveVal_A2.obj["T3"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
             showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
             showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
             showLabel[0][index2]->setFont(*settings.showlabel_font);
             //计算错误码
             for(int i=0;i<5;i++){
                 tmp=obj.Data[6]>>i&1;
                 error1+=base*tmp;
                 base*=2;
             }
             sensorState(index1,settings.errorCode_TC,error1);
             ReceiveVal_A2.obj["T2"]["status"].push_back(settings.errorCode_TC[error1]);

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
             sensorState(index2,settings.errorCode_TC,error2);
             ReceiveVal_A2.obj["T3"]["status"].push_back(settings.errorCode_TC[error2]);
             break;
        case 0x18FF7AD4: //0x18FF7AD4 A2 CJ  μC
            if(company_type){
                return;
            }
            index1=2*4+1;
            index2=2*5+1;
            //计算数值和时间
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

            ReceiveVal_A2.obj["CJ"]["hightempTest"].push_back(".");
            ReceiveVal_A2.obj["μC"]["precisionTest"].push_back(".");

            emit sendtochart_Second_Down(mx/4,1,Tcf);
            emit sendtochart_Second_Down(mx/4,2,Tcs);
            //保存数据
            ReceiveVal_A2.obj["CJ"]["val"].push_back(QString::number(Tcf,'f',2));
            //计算时间
            tp=Calculate(index1,Tcf);
            ReceiveVal_A2.obj["CJ"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            showLabel[0][index1]->setFont(*settings.showlabel_font);

            ReceiveVal_A2.obj["μC"]["val"].push_back(QString::number(Tcs,'f',2));
            tp=Calculate(index2,Tcs);
            ReceiveVal_A2.obj["μC"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            showLabel[0][index2]->setFont(*settings.showlabel_font);
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
            sensorState(index1,settings.errorCode_CJ,error1);
            sensorState(index2,settings.errorCode_ECU,error2);
            ReceiveVal_A2.obj["CJ"]["status"].push_back(settings.errorCode_CJ[error1]);
            ReceiveVal_A2.obj["μC"]["status"].push_back(settings.errorCode_ECU[error2]);

            break;

        case 0x10FD4784:   //0x10FD4784   测试电压值
            if(company_type){
                return;
            }
            index1=2*6+1;
            index2=2*7+1;
            index3=2*8+1;
            index4=2*9+1;
            V1=(obj.Data[1]*256+obj.Data[0])*1.0/200-100;
            V2=(obj.Data[3]*256+obj.Data[2])*1.0/200-100;
            V3=(obj.Data[5]*256+obj.Data[4])*1.0/200-100;
            V4=(obj.Data[7]*256+obj.Data[6])*1.0/200-100;
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,V1),'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,V2),'f',2));
            showLabel[0][index3]->setText(QString::number(lowPassFilter(index3,V3),'f',2));
            showLabel[0][index4]->setText(QString::number(lowPassFilter(index4,V4),'f',2));
            break;

        ///*
        /// B1
        /// *
        case 0x18FF76D3:  //0x18FF76D3  B1 T3  T4
            if(!company_type){
                return;
            }
            //计算数值和时间
            index1=2*2;
            index2=2*3;
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

            if(IfTestHighTemp==true){
                ReceiveVal_B1.obj["T3"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index1,Tcf)]);
                ReceiveVal_B1.obj["T4"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index2,Tcs)]);
            }else{
                ReceiveVal_B1.obj["T3"]["hightempTest"].push_back(settings.Test_status[2]);
                ReceiveVal_B1.obj["T4"]["hightempTest"].push_back(settings.Test_status[2]);
            }
            if(IfTestPrecision==true){
                ReceiveVal_B1.obj["T3"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
                ReceiveVal_B1.obj["T4"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            }else{
                ReceiveVal_B1.obj["T3"]["precisionTest"].push_back(settings.Test_status[2]);
                ReceiveVal_B1.obj["T4"]["precisionTest"].push_back(settings.Test_status[2]);
            }
            if(!IfTestHighTemp&&!IfTestPrecision){
                setnotest(index1);
                setnotest(index2);
            }

            emit sendtochart_First_Up(mx/4,2,Tcf);
            emit sendtochart_First_Down(mx/4,0,Tcs);
            //保存数据
            ReceiveVal_B1.obj["T3"]["val"].push_back(QString::number(Tcf,'f',2));
            //计算时间
            tp=Calculate(index1,Tcf);
            ReceiveVal_B1.obj["T3"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            showLabel[0][index1]->setFont(*settings.showlabel_font);

            ReceiveVal_B1.obj["T4"]["val"].push_back(QString::number(Tcs,'f',2));
            tp=Calculate(index2,Tcs);
            ReceiveVal_B1.obj["T4"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            showLabel[0][index2]->setFont(*settings.showlabel_font);
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
            sensorState(index1,settings.errorCode_TC,error1);
            sensorState(index2,settings.errorCode_TC,error2);
            ReceiveVal_B1.obj["T3"]["status"].push_back(settings.errorCode_TC[error1]);
            ReceiveVal_B1.obj["T4"]["status"].push_back(settings.errorCode_TC[error2]);
            ReceiveTimestamp[0].push_back(datetime);
            break;
         case 0x18FF77D3:  //0x18FF77D3 B1 CJ  μC
             if(!company_type){
                 return;
             }
             index1=2*4;
             index2=2*5;
             Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
             Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

             ReceiveVal_B1.obj["CJ"]["hightempTest"].push_back(".");
             ReceiveVal_B1.obj["μC"]["precisionTest"].push_back(".");

             emit sendtochart_First_Down(mx/4,1,Tcf);
             emit sendtochart_First_Down(mx/4,2,Tcs);
             //保存数据
             ReceiveVal_B1.obj["CJ"]["val"].push_back(QString::number(Tcf,'f',2));
             //计算时间
             tp=Calculate(index1,Tcf);
             ReceiveVal_B1.obj["CJ"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
             showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
             showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
             showLabel[0][index1]->setFont(*settings.showlabel_font);

             ReceiveVal_B1.obj["μC"]["val"].push_back(QString::number(Tcs,'f',2));
             tp=Calculate(index2,Tcs);
             ReceiveVal_B1.obj["μC"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
             showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
             showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
             showLabel[0][index2]->setFont(*settings.showlabel_font);
             //计算错误码
             for(int i=0;i<5;i++){
                 tmp=obj.Data[6]>>i&1;
                 error1+=base*tmp;
                 base*=2;
             }
             sensorState(index1,settings.errorCode_CJ,error1);
             ReceiveVal_B1.obj["CJ"]["status"].push_back(settings.errorCode_ECU[error1]);

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
             sensorState(index2,settings.errorCode_ECU,error2);
             ReceiveVal_B1.obj["μC"]["status"].push_back(settings.errorCode_ECU[error2]);
             break;

        case 0x18FF74D3: //0x18FF74D3  B1 T1 T2
            if(!company_type){
                return;
            }
            index1=2*0;
            index2=2*1;

            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

            if(IfTestHighTemp==true){
                ReceiveVal_B1.obj["T1"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index1,Tcf)]);
                ReceiveVal_B1.obj["T2"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index2,Tcs)]);
            }else{
                ReceiveVal_B1.obj["T1"]["hightempTest"].push_back(settings.Test_status[2]);
                ReceiveVal_B1.obj["T2"]["hightempTest"].push_back(settings.Test_status[2]);
            }
            if(IfTestPrecision==true){
                ReceiveVal_B1.obj["T1"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
                ReceiveVal_B1.obj["T2"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            }else{
                ReceiveVal_B1.obj["T1"]["precisionTest"].push_back(settings.Test_status[2]);
                ReceiveVal_B1.obj["T2"]["precisionTest"].push_back(settings.Test_status[2]);
            }
            if(!IfTestHighTemp&&!IfTestPrecision){
                setnotest(index1);
                setnotest(index2);
            }

            emit sendtochart_First_Up(mx/4,0,Tcf);
            emit sendtochart_First_Up(mx/4,1,Tcs);

            error2=0;
            base=1;
            ReceiveVal_B1.obj["T1"]["val"].push_back(QString::number(Tcf,'f',2));
            //计算时间
            tp=Calculate(index1,Tcf);
            ReceiveVal_B1.obj["T1"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            showLabel[0][index1]->setFont(*settings.showlabel_font);
            for(int i=0;i<5;i++){
                tmp=obj.Data[6]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            sensorState(index1,settings.errorCode_TC,error2);
            ReceiveVal_B1.obj["T1"]["status"].push_back(settings.errorCode_TC[error2]);

            error2=0;
            base=1;
            ReceiveVal_B1.obj["T2"]["val"].push_back(QString::number(Tcs,'f',2));
            tp=Calculate(index2,Tcs);
            ReceiveVal_B1.obj["T2"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            showLabel[0][index2]->setFont(*settings.showlabel_font);
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
            sensorState(index2,settings.errorCode_TC,error2);
            ReceiveVal_B1.obj["T2"]["status"].push_back(settings.errorCode_TC[error2]);
            break;

        case 0x10FD47D3:   //0x10FD47D3   //测试用数据帧，热电偶电压值
            if(!company_type){
                return;
            }
            index1=2*6;
            index2=2*7;
            index3=2*8;
            index4=2*9;
            V1=(obj.Data[1]*256+obj.Data[0])*1.0/200-100;
            V2=(obj.Data[3]*256+obj.Data[2])*1.0/200-100;
            V3=(obj.Data[5]*256+obj.Data[4])*1.0/200-100;
            V4=(obj.Data[7]*256+obj.Data[6])*1.0/200-100;
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,V1),'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,V2),'f',2));
            showLabel[0][index3]->setText(QString::number(lowPassFilter(index3,V3),'f',2));
            showLabel[0][index4]->setText(QString::number(lowPassFilter(index4,V4),'f',2));
             break;
        ///*
        /// B2
        ///*
        case 0x18FF76D4: //0x18FF76D4 B2 T3  T4
            if(!company_type){
                return;
            }
            //计算数值和时间
            index1=2*2+1;
            index2=2*3+1;
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

            if(IfTestHighTemp==true){
                ReceiveVal_B2.obj["T3"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index1,Tcf)]);
                ReceiveVal_B2.obj["T4"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index2,Tcs)]);
            }else{
                ReceiveVal_B2.obj["T3"]["hightempTest"].push_back(settings.Test_status[2]);
                ReceiveVal_B2.obj["T4"]["hightempTest"].push_back(settings.Test_status[2]);
            }
            if(IfTestPrecision==true){
                ReceiveVal_B2.obj["T3"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
                ReceiveVal_B2.obj["T4"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            }else{
                ReceiveVal_B2.obj["T3"]["precisionTest"].push_back(settings.Test_status[2]);
                ReceiveVal_B2.obj["T4"]["precisionTest"].push_back(settings.Test_status[2]);
            }
            if(!IfTestHighTemp&&!IfTestPrecision){
                setnotest(index1);
                setnotest(index2);
            }

            emit sendtochart_Second_Up(mx/4,2,Tcf);
            emit sendtochart_Second_Down(mx/4,0,Tcs);
            //保存数据
            ReceiveVal_B2.obj["T3"]["val"].push_back(QString::number(Tcf,'f',2));
            //计算时间
            tp=Calculate(index1,Tcf);
            ReceiveVal_B2.obj["T3"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            showLabel[0][index1]->setFont(*settings.showlabel_font);

            ReceiveVal_B2.obj["T4"]["val"].push_back(QString::number(Tcs,'f',2));
            tp=Calculate(index2,Tcs);
            ReceiveVal_B2.obj["T4"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            showLabel[0][index2]->setFont(*settings.showlabel_font);
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
            sensorState(index1,settings.errorCode_TC,error1);
            sensorState(index2,settings.errorCode_TC,error2);

            ReceiveVal_B2.obj["T3"]["status"].push_back(settings.errorCode_TC[error1]);
            ReceiveVal_B2.obj["T4"]["status"].push_back(settings.errorCode_TC[error2]);

            ReceiveTimestamp[1].push_back(datetime);
            break;
         case 0x18FF77D4:  //0x18FF77D4 B2 CJ  μC
             if(!company_type){
                 return;
             }
             index1=2*4+1;
             index2=2*5+1;
             Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
             Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

             ReceiveVal_B2.obj["CJ"]["hightempTest"].push_back(".");
             ReceiveVal_B2.obj["μC"]["precisionTest"].push_back(".");

             emit sendtochart_Second_Down(mx/4,1,Tcf);
             emit sendtochart_Second_Down(mx/4,2,Tcs);

             //保存数据
             ReceiveVal_B2.obj["CJ"]["val"].push_back(QString::number(Tcf,'f',2));
             //计算时间
             tp=Calculate(index1,Tcf);
             ReceiveVal_B2.obj["CJ"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
             showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
             showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
             showLabel[0][index1]->setFont(*settings.showlabel_font);

             ReceiveVal_B2.obj["μC"]["val"].push_back(QString::number(Tcs,'f',2));
             tp=Calculate(index2,Tcs);
             ReceiveVal_B2.obj["μC"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
             showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
             showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
             showLabel[0][index2]->setFont(*settings.showlabel_font);
             //计算错误码
             for(int i=0;i<5;i++){
                 tmp=obj.Data[6]>>i&1;
                 error1+=base*tmp;
                 base*=2;
             }
             sensorState(index1,settings.errorCode_CJ,error1);
             ReceiveVal_B2.obj["CJ"]["status"].push_back(settings.errorCode_ECU[error1]);

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
             sensorState(index2,settings.errorCode_ECU,error2);
             ReceiveVal_B2.obj["μC"]["status"].push_back(settings.errorCode_ECU[error2]);
             break;

        case 0x18FF74D4:  //0x18FF74D4 B2 T1 T2
            if(!company_type){
                return;
            }
            index1=2*0+1;
            index2=2*1+1;
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

            if(IfTestHighTemp==true){
                ReceiveVal_B2.obj["T1"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index1,Tcf)]);
                ReceiveVal_B2.obj["T2"]["hightempTest"].push_back(settings.Test_status[highTempStableTest(index2,Tcs)]);
            }else{
                ReceiveVal_B2.obj["T1"]["hightempTest"].push_back(settings.Test_status[2]);
                ReceiveVal_B2.obj["T2"]["hightempTest"].push_back(settings.Test_status[2]);
            }
            if(IfTestPrecision==true){
                ReceiveVal_B2.obj["T1"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index1,Tcf)]);
                ReceiveVal_B2.obj["T2"]["precisionTest"].push_back(settings.Test_status[PrecesionTest(index2,Tcs)]);
            }else{
                ReceiveVal_B2.obj["T1"]["precisionTest"].push_back(settings.Test_status[2]);
                ReceiveVal_B2.obj["T2"]["precisionTest"].push_back(settings.Test_status[2]);
            }
            if(!IfTestHighTemp&&!IfTestPrecision){
                setnotest(index1);
                setnotest(index2);
            }

            emit sendtochart_Second_Up(mx/4,0,Tcf);
            emit sendtochart_Second_Up(mx/4,1,Tcs);

            ReceiveVal_B2.obj["T1"]["val"].push_back(QString::number(Tcf,'f',2));
            //计算时间
            tp=Calculate(index1,Tcf);
            ReceiveVal_B2.obj["T1"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index1]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            showLabel[0][index1]->setFont(*settings.showlabel_font);
            for(int i=0;i<5;i++){
                tmp=obj.Data[6]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            sensorState(index1,settings.errorCode_TC,error2);
            ReceiveVal_B2.obj["T1"]["status"].push_back(settings.errorCode_TC[error2]);

            error2=0;
            base=1;
            ReceiveVal_B2.obj["T2"]["val"].push_back(QString::number(Tcs,'f',2));
            tp=Calculate(index2,Tcs);
            ReceiveVal_B2.obj["T2"]["time"].push_back(QString::number(tp>0?tp:0,'f',2));
            showLabel[2][index2]->setText(QString::number(tp>0?tp:0,'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            showLabel[0][index2]->setFont(*settings.showlabel_font);
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
            sensorState(index2,settings.errorCode_TC,error2);
            ReceiveVal_B2.obj["T2"]["status"].push_back(settings.errorCode_TC[error2]);
            break;

        case 0x10FD47D4:   //0x10FD47D4  B2 测试用数据帧，热电偶电压值
            if(!company_type){
                return;
            }
            index1=2*6+1;
            index2=2*7+1;
            index3=2*8+1;
            index4=2*9+1;
            V1=(obj.Data[1]*256+obj.Data[0])*1.0/200-100;
            V2=(obj.Data[3]*256+obj.Data[2])*1.0/200-100;
            V3=(obj.Data[5]*256+obj.Data[4])*1.0/200-100;
            V4=(obj.Data[7]*256+obj.Data[6])*1.0/200-100;
            showLabel[0][index1]->setText(QString::number(lowPassFilter(index1,V1),'f',2));
            showLabel[0][index2]->setText(QString::number(lowPassFilter(index2,V2),'f',2));
            showLabel[0][index3]->setText(QString::number(lowPassFilter(index3,V3),'f',2));
            showLabel[0][index4]->setText(QString::number(lowPassFilter(index4,V4),'f',2));
            break;
        default: break;
    }
}


Q_DECLARE_METATYPE(QVector<QVector<QString>>)
Q_DECLARE_METATYPE(QVector<QString>)
Q_DECLARE_METATYPE(QVector<QVector<double>>)

//Q_DECLARE_METATYPE(QMap<QString,QMap<QString,QMap<QString,QVector<QString>>>>)
void TestDisplay::handleTimeOut(){
        //这部分是公用的
        double newtime=GetTickCount();
        if(newtime-saveTime>saveInterval_miseconds){
            int index=combobox[0]->currentIndex();

            //swap 速度快，且清空
            if(index==0){
                //A
                dbDoubleChart *obj=new dbDoubleChart(ReceiveVal_A1.obj,
                                                     ReceiveVal_A2.obj,
                                                     ReceiveTimestamp,
                                                     QDateTime::currentDateTime().toString("yyyy_MM_dd"),
                                                     combobox[0]->currentText());
                ReceiveVal_A1.reset();
                ReceiveVal_A2.reset();
                ReceiveTimestamp.resize(2);
                dbcontroller.addObject(obj);
            }
            else if(index==1){
                //B
                dbDoubleChart *obj=new dbDoubleChart(ReceiveVal_B1.obj,
                                                     ReceiveVal_B2.obj,
                                                     ReceiveTimestamp,
                                                     QDateTime::currentDateTime().toString("yyyy_MM_dd"),
                                                     combobox[0]->currentText());
                ReceiveVal_B1.reset();
                ReceiveVal_B2.reset();
                ReceiveTimestamp.resize(2);
                dbcontroller.addObject(obj);
            }
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
    qDebug()<<"clicked  "<<index<<"  turn to "<<selected;
    if(index<3){
       mycharts[0]->show_hidden_Series(index,selected);
       mycharts[2]->show_hidden_Series(index,selected);

    }else{
       mycharts[1]->show_hidden_Series(index-3,selected);
       mycharts[3]->show_hidden_Series(index-3,selected);
    }
}

void TestDisplay::change_Company(int index){
    qDebug()<<index;
    companyname=combobox[0]->currentText();
    company_type=index;
    for (int i=0;i<mycharts.size();i++) {
        mycharts[i]->changeSplineName(index);
    }   
    for (int i=0;i<checkboxs.size();i++) {
        checkboxs[i]->setText(company_date[i]);
    }
    for(int i=0;i<gridlayout2.size();i++){
        ((QLabel*)(gridlayout2[i]->itemAtPosition(0,0)->widget()))->setText(combobox[0]->currentText()+"1");
        ((QLabel*)(gridlayout2[i]->itemAtPosition(0,1)->widget()))->setText(combobox[0]->currentText()+"2");
    }
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
            combobox[0]->setEnabled(false);
            saveTime=GetTickCount();
            pushbuttons[buttonmap["opendevice"]]->setText("停止测试");
        }
        else{
            IfRun=false;
            timer->stop();
            combobox[0]->setEnabled(true);
            pushbuttons[buttonmap["opendevice"]]->setText("开启测试");
        }
    }
    else{
        if(IfRun==false){
            DWORD drl;
            drl=usbcanunion.startUsbCan();
            if (drl != STATUS_OK)
            {
                QMessageBox::information(this,"提示","开启失败",QMessageBox::Warning);
                return ;
            }else{
                IfRun=true;
                emit usbcanunion.setFrameStartTime();
                timer->start(100);
                //重新技术
                combobox[0]->setEnabled(false);
                saveTime=GetTickCount();
                pushbuttons[buttonmap["opendevice"]]->setText("停止测试");
            }
        }else{
            IfRun=false;
            timer->stop();
            combobox[0]->setEnabled(true);
            pushbuttons[buttonmap["opendevice"]]->setText("开启测试");
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
    double ymin=lineedit[lineeditmap["xaxis"]]->text().toDouble();
    double ymax=lineedit[lineeditmap["yaxis"]]->text().toDouble();

    for (int i=0;i<mycharts.size();i++) {
        qDebug()<<ymin<<"  "<<ymax;
        mycharts[i]->changeYScale(ymin,ymax);
    }
}

void TestDisplay::highTempTestSwitch(bool state){
    qDebug()<<state;
    lineedit[lineeditmap["hightemp"]]->setEnabled(state);
    this->IfTestHighTemp=state;
    int size=this->highTempStatus.size();
    for (int i=0;i<size;i++) {
        highTempStatus[i]=true;
        highTempTest[i]=0;
    }
}

void TestDisplay::precisionTestSwitch(bool state){
    this->IfTestPrecision=state;
    if(state){
        QString &&tmp=lineedit[lineeditmap["precesionval"]]->text();
        TestTemperature=tmp.simplified()==""?20:tmp.toDouble();
        if(TestTemperature<500){
            offset=-4;
        }
        else{
            offset=0.008;
        }
        tmp=lineedit[lineeditmap["precesiontime"]]->text();
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

void TestDisplay::getPrecesion(){

}

void TestDisplay::getPrePrecesionTime(){

}

void TestDisplay::handlePrePrecesionTimeOut(){
    IfTestPrePrecesion=false;
    pretimer.stop();
}

void TestDisplay::getHighThreshold(){
    settings.HightempThreshold=lineedit[lineeditmap["hightemp"]]->text().toInt();
}


void TestDisplay::newscan(){

    scangun=new ScanGun(this,&settings.eightChannelSnCode);
    //关闭是删除
    scangun->setAttribute(Qt::WA_DeleteOnClose);
    scangun->show();
}

void TestDisplay::senddata(){
    MqttDataStruct data;
     if(highTempCheckBox->isChecked()){
         data.AddTestName(0);
     }else{
         data.AddTestName(2);
     }
    int size=settings.eightChannelSnCode.size();
    int ary[8]={0,2,4,6,1,3,5,7};
    for (int i=0;i<size;i++) {
        QString sensor_name="sensor_"+QString::number(i);
        data.AddObject(sensor_name,"sensorId",settings.eightChannelSnCode[i]);
        if(highTempCheckBox->isChecked()){
            data.AddObject(sensor_name,"highTempTest",highTempStatus[ary[i]]?"Pass":"Error");
        }
        if(precisionCheckBox->isChecked()){
            data.AddObject(sensor_name,"precisionTest",precisionStatus[ary[i]]?"Pass":"Error");
            data.AddObject(sensor_name,"precisionError",QString::number(precisionError[ary[i]]));
        }
    }
    mqtt.publish_Mes(data);
}

QVector<QString>* TestDisplay::getScanMes(){
    return &settings.eightChannelSnCode;
}
