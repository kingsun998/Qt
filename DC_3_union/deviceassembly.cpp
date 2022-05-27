#include "deviceassembly.h"
#include "ui_deviceassembly.h"
#include "settings.h"
#include "QVBoxLayout"
#include "QChartView"
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <usbcanunion.h>
#include <mqttcommunication.h>
deviceAssembly::deviceAssembly(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deviceAssembly)
{
    ui->setupUi(this);

    mychart =new Mychart(0,0,0,6,0);
    ui->horizontalLayout_3->addWidget(new QChartView(mychart));


    buttongroup=new QButtonGroup();
    QVBoxLayout *vlayout=new QVBoxLayout();
    for(int j=0;j<settings.probeName.size();j++){
        QCheckBox *checkbox=new QCheckBox(settings.probeName[j]);
        checkboxList.append(checkbox);
        buttongroup->addButton(checkbox);
        vlayout->addWidget(checkbox);
    }
    buttongroup->setExclusive(false);
    for(int i=0;i<checkboxList.size();i++){
        checkboxList[i]->setChecked(true);
    }
    ui->groupBox->setLayout(vlayout);

    vlayout=new QVBoxLayout();
    for(int j=0;j<settings.probeName.size();j++){
        QLabel *label=new QLabel("...");
        testValue.append(label);
        vlayout->addWidget(label);
    }
    ui->groupBox_2->setLayout(vlayout);

    vlayout=new QVBoxLayout();
    for(int j=0;j<settings.probeName.size();j++){
        QLabel *label=new QLabel("...");
        diffValue.append(label);
        vlayout->addWidget(label);
    }
    ui->groupBox_3->setLayout(vlayout);

    vlayout=new QVBoxLayout();
    for(int j=0;j<settings.probeName.size();j++){
        QLabel *label=new QLabel("...");
        testState.append(label);
        vlayout->addWidget(label);
    }
    ui->groupBox_4->setLayout(vlayout);

    vlayout=new QVBoxLayout();
    for(int j=0;j<settings.probeName.size();j++){
        QLabel *label=new QLabel("...");
        sensorState.append(label);
        vlayout->addWidget(label);
    }
    ui->groupBox_5->setLayout(vlayout);

    idlabel.append(ui->label_2);
    idlabel.append(ui->label_4);
    idlabel.append(ui->label_6);
    idlabel.append(ui->label_8);
    idlabel.append(ui->label_10);

    ui->label->setFont(*settings.showlabel_font);
    ui->label_3->setFont(*settings.showlabel_font);
    ui->label_5->setFont(*settings.showlabel_font);
    ui->label_7->setFont(*settings.showlabel_font);
    ui->label_9->setFont(*settings.showlabel_font);

    connect(buttongroup,SIGNAL(buttonToggled(int,bool)),this,SLOT(shspline(int,bool)));
    connect(ui->comboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&deviceAssembly::change_Company);
    connect(this,&deviceAssembly::sendMessage,this,&deviceAssembly::show_detail);
    connect(this,&deviceAssembly::sendtochart,mychart,&Mychart::getMessage);
    connect(&timer,&QTimer::timeout,this,&deviceAssembly::handleTimeOut);
    connect(ui->checkBox,&QCheckBox::clicked,this,&deviceAssembly::precisionTestSwitch);
    connect(&pretimer,&QTimer::timeout,this,&deviceAssembly::handlePreTimeOut);

    IfRun=false;
    company_name="A";
    company_type=0;
    IfTestPrecision=false;
    IfTestPrePrecesion=false;
    saveInterval_miseconds=settings.saveInterval_minus*60*1000;
    m_x=0;
    precisionStatus.resize(settings.probeName.size());
    for (int i=0;i<settings.probeName.size();i++) {
        precisionStatus[i]=true;
    }

    recordVal.resize(8);
    show_clock.resize(8);
    recordValTemp.resize(8);
    precisionError.resize(8);
//    precisionDiff.resize(8);

    rowcount=-1;

    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}

deviceAssembly::~deviceAssembly()
{
    delete ui;
}

void deviceAssembly::shspline(int index,bool checked){
    index=-index-2;
    qDebug()<<index;
    mychart->show_hidden_Series(index,checked);
}
void deviceAssembly::connectCan(){

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

void deviceAssembly::startCan(){
    qDebug()<<"测试模式："<<settings.TestMode;
    if(settings.TestMode){
        if(IfRun==false){
            //    测试代码
            IfRun=true;
            emit usbcanunion.setFrameStartTime();
            timer.start(settings.timer_interval);

            ui->comboBox->setEnabled(false);
            saveTime=GetTickCount();
            ui->pushButton_5->setText("停止测试");
        }
        else{
            IfRun=false;
            timer.stop();
            ui->comboBox->setEnabled(true);
            ui->pushButton_5->setText("开启测试");
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
                timer.start(100);
                //重新技术
                ui->comboBox->setEnabled(false);
                saveTime=GetTickCount();
                ui->pushButton_5->setText("停止测试");
            }
        }else{
            IfRun=false;
            timer.stop();
            ui->comboBox->setEnabled(true);
            ui->pushButton_5->setText("开启测试");
        }
    }
}

void deviceAssembly::disconnectCan(){
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
void deviceAssembly::precisionTestSwitch(bool state){
    this->IfTestPrecision=state;
    if(state){
        QString &&tmp=ui->lineEdit_2->text();
        TestTemperature=tmp.simplified()==""?20:tmp.toDouble();
        if(TestTemperature<500){
            offset=-4;
        }
        else{
            offset=0.008;
        }
        tmp=ui->lineEdit_3->text();
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
void deviceAssembly::YscaleJump(){
    double ymin=ui->lineEdit_2->text().toDouble();
    double ymax=ui->lineEdit_3->text().toDouble();
    mychart->changeYScale(ymin,ymax);
}
void deviceAssembly::change_Company(int index){
    company_name=ui->comboBox->currentText();
    company_type=index;
    m_x=0;
}

double deviceAssembly::lowPassFilter(int index,double val){
    recordVal[index]=0.5*(recordVal[index]+val);
    show_clock[index]=(++show_clock[index])%settings.show_clock;
    recordValTemp[index]=show_clock[index]==0?recordVal[index]:recordValTemp[index];
    return recordValTemp[index];
}
int deviceAssembly::PrecesionTest(int index,double val){
    QPalette ps;
    bool flg;
    if(precisionStatus[index]){
        if(!IfTestPrePrecesion){
            val=lowPassFilter(index,val);
            precisionError[index]=val-TestTemperature;
            diffValue[index]->setText(QString::number(precisionError[index]));
            if(offset>0){
                flg=abs(precisionError[index])<TestTemperature*(offset);
            }else{
                flg=abs(precisionError[index])<abs(offset);
            }
            int idx=(flg==true?1:0);
//            precisionDiff[index]=idx;
            ps.setColor(QPalette::WindowText,settings.Test_status_color[idx]);
            testState[index]->setText(settings.Test_status[idx]);
            testState[index]->setPalette(ps);
            testState[index]->setFont(*settings.showlabel_font);
            precisionStatus[index]=flg;
            return idx;
        }
        else{
            ps.setColor(QPalette::WindowText,settings.Test_status_color[2]);
            testState[index]->setText("...");
            testState[index]->setPalette(ps);
            return 3;
        }

    }
    return 0;
}
void deviceAssembly::setSensorState(int index,std::map<int,QString>& map,int val){
     QPalette ps;
     if(val==0){
        ps.setColor(QPalette::WindowText,settings.Test_status_color[1]);
     }else{
        ps.setColor(QPalette::WindowText,settings.Test_status_color[0]);
     }
     sensorState[index]->setText(map[val]);
     sensorState[index]->setPalette(ps);
}
void deviceAssembly::processTest(int index1,double Tcf,int index2,double Tcs,ulong mx){
    if(IfTestPrecision==true){
        PrecesionTest(index1,Tcf);
        PrecesionTest(index2,Tcs);
    }else{
        setnotest(index1);
        setnotest(index2);
    }
    emit sendtochart(mx/6,index1,Tcf);
    emit sendtochart(mx/6,index2,Tcs);
}

void deviceAssembly::setnotest(int index){
    QPalette ps;
    ps.setColor(QPalette::WindowText,settings.Test_status_color[2]);
    testState[index]->setText(settings.Test_status[2]);
    testState[index]->setPalette(ps);
}
void deviceAssembly::show_detail(ulong mx,CAN_OBJ obj,QString datetime){
    int error1=0;
    int error2=0;
    int base=1;
    int tmp;
    double tp;
    double Tcf,Tcs;
    int index1,index2,index3,index4;

    //发送到表格
    //emit SendToFrame(mx,obj,datetime,company_name);
    //这里使用id偏移适配两个种类
    switch (obj.ID) {
        ///*
        /// A1
        ///*
        case 0x18FD2083: //0x18FD2083 A1 T1  T4
            if(company_type){
                return;
            }
            index1=0;
            index2=3;
            //计算数值和时间
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;
            qDebug()<<Tcf<<" "<<Tcs;
            processTest(index1,Tcf,index2,Tcs,mx);

            testValue[index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            testValue[index1]->setFont(*settings.showlabel_font);

            testValue[index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            testValue[index2]->setFont(*settings.showlabel_font);
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
            setSensorState(index1,settings.errorCode_TC,error1);
            setSensorState(index2,settings.errorCode_TC,error2);
            break;

         case 0x14FD3E83:  //0x14FD3E83 A1 T2  T3
             if(company_type){
                 return;
             }
             index1=1;
             index2=2;
             Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
             Tcs=(obj.Data[4]*256+obj.Data[3])*0.03125-273;

             processTest(index1,Tcf,index2,Tcs,mx);

             testValue[index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
             testValue[index1]->setFont(*settings.showlabel_font);

             testValue[index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
             testValue[index2]->setFont(*settings.showlabel_font);
             //计算错误码
             for(int i=0;i<5;i++){
                 tmp=obj.Data[2]>>i&1;
                 error1+=base*tmp;
                 base*=2;
             }
             setSensorState(index1,settings.errorCode_TC,error1);

             base=1;
             for(int i=0;i<5;i++){
                 tmp=obj.Data[5]>>i&1;
                 error2+=base*tmp;
                 base*=2;
             }
             setSensorState(index2,settings.errorCode_TC,error2);
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


            processTest(index1,Tcf,index2,Tcs,mx);

            testValue[index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            testValue[index1]->setFont(*settings.showlabel_font);

            testValue[index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            testValue[index2]->setFont(*settings.showlabel_font);
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
            setSensorState(index1,settings.errorCode_CJ,error1);
            setSensorState(index2,settings.errorCode_ECU,error2);

            break;
        ///*
        /// B1
        /// *
        case 0x18FF76D3:  //0x18FF76D3  B1 T3  T4
            if(!company_type){
                return;
            }
            //计算数值和时间
            index1=2;
            index2=3;
            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

            processTest(index1,Tcf,index2,Tcs,mx);

            testValue[index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            testValue[index1]->setFont(*settings.showlabel_font);

            testValue[index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            testValue[index2]->setFont(*settings.showlabel_font);
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
            setSensorState(index1,settings.errorCode_TC,error1);
            setSensorState(index2,settings.errorCode_TC,error2);
            break;
         case 0x18FF77D3:  //0x18FF77D3 B1 CJ  μC
             if(!company_type){
                 return;
             }
             index1=4;
             index2=5;
             Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
             Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

             emit sendtochart(mx/6,index1,Tcf);
             emit sendtochart(mx/6,index2,Tcs);

             testState[index1]->setText(QString::number(tp>0?tp:0,'f',2));
             testValue[index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
             testValue[index1]->setFont(*settings.showlabel_font);


             testState[index2]->setText(QString::number(tp>0?tp:0,'f',2));
             testValue[index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
             testValue[index2]->setFont(*settings.showlabel_font);
             //计算错误码
             for(int i=0;i<5;i++){
                 tmp=obj.Data[6]>>i&1;
                 error1+=base*tmp;
                 base*=2;
             }
             setSensorState(index1,settings.errorCode_CJ,error1);

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
             setSensorState(index2,settings.errorCode_ECU,error2);
             break;

        case 0x18FF74D3: //0x18FF74D3  B1 T1 T2
            if(!company_type){
                return;
            }
            index1=0;
            index2=1;

            Tcf=(obj.Data[1]*256+obj.Data[0])*0.03125-273;
            Tcs=(obj.Data[3]*256+obj.Data[2])*0.03125-273;

            processTest(index1,Tcf,index2,Tcs,mx);

            error2=0;
            base=1;

            testValue[index1]->setText(QString::number(lowPassFilter(index1,Tcf),'f',2)+"  ");
            testValue[index1]->setFont(*settings.showlabel_font);
            for(int i=0;i<5;i++){
                tmp=obj.Data[6]>>i&1;
                error2+=base*tmp;
                base*=2;
            }
            setSensorState(index1,settings.errorCode_TC,error2);

            error2=0;
            base=1;

            testValue[index2]->setText(QString::number(lowPassFilter(index2,Tcs),'f',2)+"  ");
            testValue[index2]->setFont(*settings.showlabel_font);
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
            setSensorState(index2,settings.errorCode_TC,error2);
            break;
    }
}


Q_DECLARE_METATYPE(QVector<QVector<QString>>)
Q_DECLARE_METATYPE(QVector<QString>)
Q_DECLARE_METATYPE(QVector<QVector<double>>)

//Q_DECLARE_METATYPE(QMap<QString,QMap<QString,QMap<QString,QVector<QString>>>>)
void deviceAssembly::handleTimeOut(){
        //这部分是公用的

        double newtime=GetTickCount();
//        if(newtime-saveTime>saveInterval_miseconds){
//            int index=ui->comboBox->currentIndex();
//            //swap 速度快，且清空
//            if(index==0){
//                //A
//                dbDoubleChart *obj=new dbDoubleChart(ReceiveVal_A1.obj,
//                                                     ReceiveVal_A2.obj,
//                                                     ReceiveTimestamp,
//                                                     QDateTime::currentDateTime().toString("yyyy_MM_dd"),
//                                                     combobox[0]->currentText());
//                ReceiveVal_A1.reset();
//                ReceiveVal_A2.reset();
//                ReceiveTimestamp.resize(2);
//                dbcontroller.addObject(obj);
//            }
//            else if(index==1){
//                //B
//                dbDoubleChart *obj=new dbDoubleChart(ReceiveVal_B1.obj,
//                                                     ReceiveVal_B2.obj,
//                                                     ReceiveTimestamp,
//                                                     QDateTime::currentDateTime().toString("yyyy_MM_dd"),
//                                                     combobox[0]->currentText());
//                ReceiveVal_B1.reset();
//                ReceiveVal_B2.reset();
//                ReceiveTimestamp.resize(2);
//                dbcontroller.addObject(obj);
//            }
//            saveTime=newtime;
//            QDateTime end=QDateTime::currentDateTime();
//        }

        if(settings.TestMode){
            ULONG len=usbcanunion.StartUnionTestMode();
            for(uint i=0;i<len;i++){
                emit sendMessage(m_x+i,usbcanunion.objs[i],QDateTime::currentDateTime().toString("yyyy_MM_dd hh:mm:ss:zzz"));
            }
            m_x+=len;
        }
        else{
            ULONG len=usbcanunion.ReceiveFromCan();
            for(uint i=0;i<len;i++){
                emit sendMessage(m_x+i,usbcanunion.objs[i],QDateTime::currentDateTime().toString("yyyy_MM_dd hh:mm:ss:zzz"));
            }
            m_x+=len;
        }
}
void deviceAssembly::handlePreTimeOut(){
    IfTestPrePrecesion=false;
    pretimer.stop();
}
void deviceAssembly::on_pushButton_6_clicked()
{
    connectCan();
}

void deviceAssembly::on_pushButton_5_clicked()
{
    startCan();
}

void deviceAssembly::on_pushButton_4_clicked()
{
    disconnectCan();
}

void deviceAssembly::on_lineEdit_4_editingFinished()
{
    QString min=ui->lineEdit_5->text();
    QString max=ui->lineEdit_4->text();
    if(max==""){
        return;
    }
    double ymin,ymax;
    if(min==""){
        ymin=20;
    }else{
        ymin=min.toDouble();
    }
    ymax=max.toDouble();
    mychart->changeYScale(ymin,ymax);
}

void deviceAssembly::on_lineEdit_5_editingFinished()
{
    QString min=ui->lineEdit_5->text();
    QString max=ui->lineEdit_4->text();
    if(min==""){
        return;
    }
    double ymin,ymax;
    if(max==""){
        ymax=20;
    }else{
        ymax=max.toDouble();
    }
    ymin=min.toDouble();
    mychart->changeYScale(ymin,ymax);
}

void deviceAssembly::on_lineEdit_textChanged(const QString &arg1)
{
    qDebug()<<arg1;
    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(nowtime-pre_Time>30){
       sl.clear();
       sl.push_back(*(arg1.end()-1));
    }else{
       sl.push_back(*(arg1.end()-1));
    }
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}


void deviceAssembly::on_lineEdit_returnPressed()
{

    long long nowtime=QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(nowtime-pre_Time>20){
        sl.clear();
    }else{
        char ary[sl.size()];
        for(int i=0;i<sl.size();i++){
            ary[i]=sl[i].toLatin1();
        }
        QString s=QString::fromLocal8Bit(ary,sl.size());
        ui->lineEdit->setText("");
        if(rowcount<idlabel.size()-1){
           rowcount++;
           idlabel[rowcount]->setText(s);
           idlabel[rowcount]->setFont(*settings.showlabel_font);
        }
        else{
            return;
        }
    }
    pre_Time=QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void deviceAssembly::on_pushButton_2_clicked()
{
    if(rowcount>=0){
        idlabel[rowcount]->clear();
        rowcount--;
    }
}

void deviceAssembly::on_pushButton_clicked()
{
    MqttDataStruct date;
    date.AddTestName(4);
    QString itemName="item";
    for (int i=0;i<idlabel.size();i++) {
        QString id;
        if(i==0){
            id="pcbID";
            date.AddObject(itemName,id,idlabel[i]->text());
            continue;
        }else{
            QString sensor_diff="sensor_diff_"+QString::number(i-1);
            id="sensor_"+QString::number(i-1);
            date.AddObject(itemName,id,idlabel[i]->text());
            date.AddObject(itemName,sensor_diff,QString::number(precisionError[i-1]));
        }
    }
    bool state=true;
    for (int i=0;i<4;i++) {
        state&=precisionStatus[i];
    }
    date.AddObject(itemName,"testState",state?"Pass":"Error");
    qDebug()<<date.tojson();
    mqtt.publish_Mes(date);
}
