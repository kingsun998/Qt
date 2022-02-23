#include "framedispaly.h"
#include "ui_framedisplay.h"
#include <memory>
#include <windows.h>
#include <mutex>
#include <subcomdialog.h>
#include <qmessagebox.h>
frameDisplay::frameDisplay(QWidget *parent) : QWidget(parent),
    ui(new Ui::framedisplay_ui)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(8);

    /* 行颜色交替显示 */
    ui->tableWidget->setAlternatingRowColors(true);
    /* 不允许在图形界面修改内容 */
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* 设置列数 */

    QStringList stringlists;
    stringlists<<"行号"<<"设备号"<<"帧类型"<<"帧方向"<<"帧ID"<<"帧长度"<<"帧内容"<<"时间戳";
    ui->tableWidget->setHorizontalHeaderLabels(stringlists);

    ui->tableWidget->setColumnWidth(6,300);
    ui->tableWidget->setColumnWidth(7,300);
    rowcount=0;
    dispaly_allowshow=true;
    dispaly_ifreceive=true;

    saveInterval_miseconds=settings.saveTable_Interval*60*1000;


    send_frame_type=settings.send_frame_type[0];
    send_frame_format=settings.send_frame_format[0];
    send_frame_id=0;
//    send_frame_interval=10000;
    send_frame_nums=1;

    //为串口设置参数
    parameter.widget=this;
    parameter.dequein=1024;
    parameter.dequeout=1024;
    parameter.datalength=128;
    //波特率
    parameter.parity=EVENPARITY;
    parameter.bitssize=7;
    parameter.stopbits=ONESTOPBIT;
    parameter.baudrate=9600;
    parameter.comindex="COM4";

    parameter.ReadIntervalTimeout=50;
    parameter.ReadTotalTimeoutConstant=10;
    parameter.ReadTotalTimeoutMultiplier=10;
    parameter.WriteTotalTimeoutConstant=30;
    parameter.WriteTotalTimeoutMultiplier=10;

    connect(&comprocess,&ComProcess::returnResult,this,&frameDisplay::dispaly_comResult);

    switchReceive=false;

    dialog=nullptr;

    if(status.init==false){
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
    }

    ui->comboBox->setCurrentIndex(8);
    ui->comboBox_2->setCurrentIndex(2);
    ui->comboBox_3->setCurrentIndex(4);
    ui->comboBox_4->setCurrentIndex(0);
    ui->comboBox_5->setCurrentIndex(0);

    sendtimer=new QTimer();
    sendtimer->setInterval(100);
    connect(sendtimer,&QTimer::timeout,this,&frameDisplay::sendHandleTimeOut);
    index=ui->spinBox->text().toInt();
}

void frameDisplay::receiveMesFromCom(QVariant var){
    QVector<BYTE> vec=var.value<QVector<BYTE>>();
    int n=vec.size();
    QString str="";
    for(int i=1;i<n-1;i++){
        str+=QString::number(int(vec[i]),16);
        str+=" ";
    }
    qDebug()<<"接收到  "<<str;
}

void frameDisplay::dispaly_comResult(QVariant var,int arraytype){
    QVector<long> vector=var.value<QVector<long>>();
    qDebug()<<"数组类型 "<<arraytype<<"   数组大小 "<<vector.size()<<vector[0]<<"   "<<vector[1];
    switch(arraytype){
        case 0:
            ui->label_39->setText(QString::number(vector[0]));
            ui->label_40->setText(QString::number(vector[1]));
            ui->label->setText(QString::number(vector[2]));
        break;
        case 1:
            ui->label_41->setText(QString::number(vector[0]));
            ui->label_2->setText(QString::number(vector[1]));
        break;
        case 2:
            ui->label_48->setText(QString::number(vector[0]));
            ui->label_45->setText(QString::number(vector[1]));
            ui->label_3->setText(QString::number(vector[2]));
            ui->label_42->setText(QString::number(vector[3]));
        break;
        case 3:
            ui->label_4->setText(QString::number(vector[0]));
            ui->label_43->setText(QString::number(vector[1]));
            ui->label_46->setText(QString::number(vector[2]));
            ui->label_49->setText(QString::number(vector[3]));
            ui->label_52->setText(QString::number(vector[4]));
        break;
        case 4:
            ui->label_53->setText(QString::number(vector[0]));
            ui->label_50->setText(QString::number(vector[1]));
            ui->label_47->setText(QString::number(vector[2]));
            ui->label_44->setText(QString::number(vector[3]));
            ui->label_38->setText(QString::number(vector[4]));
        break;
    }
}



void frameDisplay::getMessage(ulong index,CAN_OBJ obj,QString datetime,QString companyname){
    //保存信息
    if(!dispaly_ifreceive){
        return;
    }
    //判断是否超过了预设时间
    if(GetTickCount()-startTime>saveInterval_miseconds){
        this->on_pushButton_clicked();
    }
    QString messageExtern;
    if(obj.ExternFlag == 1)
    {
        messageExtern = "扩展帧";
    }
    else if(obj.ExternFlag == 0)
    {
        messageExtern = "标准帧";
    }
    FrameType.push_back(messageExtern);
    ifFrameReceive.push_back(true);

    CompanyName.push_back(companyname);
    FrameID.push_back(obj.ID);
    FrameLen.push_back(obj.DataLen);
    QString data_info = QString("%1").arg(obj.Data[0], 2, 16, QLatin1Char('0')).toUpper();
    for(int data_long=1;data_long<8;data_long++)
    {
        data_info = data_info + " "+QString("%1").arg(obj.Data[data_long], 2, 16, QLatin1Char('0')).toUpper();
    }
    FrameContent.push_back(data_info);
    Data.push_back(datetime);
    QString ReceiveOrSend;
    ReceiveOrSend="接受帧";
    //显示
   if(dispaly_allowshow==false){
       return;
   }
   ui->tableWidget->insertRow(rowcount);

   QTableWidgetItem *lineid=new QTableWidgetItem(QString::number(index,10));
   lineid->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,0,lineid);


   QTableWidgetItem *cpname=new QTableWidgetItem(companyname);
   cpname->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,1,cpname);


   QTableWidgetItem *messageExternItem=new QTableWidgetItem(messageExtern);
   messageExternItem->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,2,messageExternItem);


   QTableWidgetItem *frameReceiveOrsend=new QTableWidgetItem(ReceiveOrSend);
   frameReceiveOrsend->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,3,frameReceiveOrsend);

   QTableWidgetItem *frameID=new QTableWidgetItem(QString::number(obj.ID,16));
   frameID->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,4,frameID);

   QTableWidgetItem *framelen=new QTableWidgetItem(QString::number(obj.DataLen,10));
   framelen->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,5,framelen);

   //这里写 Data
   QTableWidgetItem *df=new QTableWidgetItem(data_info);
   df->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,6,df);

   QTableWidgetItem *dt=new QTableWidgetItem(datetime);
   dt->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,7,dt);

   if(rowcount>settings.maxrowcount){
       ui->tableWidget->removeRow(0);
   }else{
       rowcount+=1;
   }

}

void frameDisplay::on_pushButton_2_clicked(){
    if(dispaly_allowshow==true){
        ui->pushButton_2->setText("继续显示");
    }else{
        ui->pushButton_2->setText("停止显示");
    }
    dispaly_allowshow=!dispaly_allowshow;
}

void frameDisplay::setStartTime(){
    showMessage("设置了开始时间",true);
    this->startTime=GetTickCount();
}

Q_DECLARE_METATYPE(QVector<uint>);
void frameDisplay::on_pushButton_clicked(){
    //保存期间关闭接受
//    qDebug()<<&settings;
    dispaly_ifreceive=false;
    //加一层锁
    std::lock_guard<std::mutex> lock(mtx);

    dbSingleTable *obj=new dbSingleTable(FrameContent,Data,FrameID,FrameType,CompanyName,FrameLen,
                                         ifFrameReceive,QDateTime::currentDateTime().toString("yyyy_MM_dd"));
    dbcontroller.addObject(obj);
    startTime=GetTickCount();
    dispaly_ifreceive=true;
}

//开启测试按钮
void frameDisplay::on_pushButton_6_clicked(){
    status.running=true;
    status.stop=false;
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    comprocess.startComProcessTimer();
    switchReceive=!switchReceive;
}
//停止测试按钮
void frameDisplay::on_pushButton_3_clicked(){
    status.running=false;
    status.stop=true;
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_3->setEnabled(false);
    comprocess.stopComProcessTimer();
    switchReceive=!switchReceive;
}
//设定参数按钮
void frameDisplay::on_pushButton_4_clicked(){
    //这里首先必须要给dialog初始化，不然是野指针，这样delete会伤害到其他内存区域
    delete dialog;
    dialog=new SubComDialog(this);
    dialog->show();
    connect(dialog,&SubComDialog::SendComMes,this,&frameDisplay::sendMessage);
}

//初始化按钮
//
void frameDisplay::on_pushButton_5_clicked(){
    if(status.init==false){
        status.init=true;
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_3->setEnabled(false);
        comprocess.init(parameter);
    }else{
        comprocess.stopComProcessTimer();
    }

    qDebug()<<"点击初始化";
}

Q_DECLARE_METATYPE(Message)
void frameDisplay::sendMessage(QVariant var){
    Message mes=var.value<Message>();
    comprocess.sendComParameter(mes);
}

//波特率
void frameDisplay::on_comboBox_currentIndexChanged(const QString &arg1){
    parameter.baudrate=arg1.toInt();
}
//选择串口
void frameDisplay::on_comboBox_3_currentIndexChanged(const QString &arg1){
    parameter.comindex=arg1;
}
//校验位
void frameDisplay::on_comboBox_5_currentIndexChanged(const QString &arg1){
    if(arg1=="奇校验"){
        parameter.parity=ODDPARITY;
    }else{
        parameter.parity=EVENPARITY;
    }

}
//数据位大小
void frameDisplay::on_comboBox_2_currentIndexChanged(const QString &arg1){
    parameter.bitssize=arg1.toInt();
}
//停止位
void frameDisplay::on_comboBox_4_currentIndexChanged(const QString &arg1){
    parameter.stopbits=arg1.toInt();
}
//初始化Can
void frameDisplay::on_pushButton_8_clicked(){
    if(settings.caninit==false){
        //CAN没有初始化
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
        settings.caninit=true;
    }
}

void frameDisplay::on_pushButton_9_clicked(){
    DWORD drl;
    drl=CloseDevice(settings.devicetype,settings.deviceid);
    if (drl != STATUS_OK)
    {
        QMessageBox::information(this,"提示","关闭链接失败",QMessageBox::Warning);
        return ;
    }
    settings.caninit=false;
}

void frameDisplay::on_pushButton_7_clicked(){
    if(settings.caninit){
        if(ui->radioButton->isChecked()){
            //选中，循环发送
            sendtimer->start();
        }
        else{
            //发送一次
            sendCan();
        }
    }
    else{
        QMessageBox::information(this,"提示","请先初始化CAN",QMessageBox::Warning);
    }
}
//计时器，间隔为0.1s
void frameDisplay::sendHandleTimeOut(){
    sendCan();
}

//停止发送
void frameDisplay::on_pushButton_10_clicked(){
    sendtimer->stop();
}

void frameDisplay::sendCan(){
    QString mes=ui->lineEdit->text();
    QStringList mslist=mes.split(" ");
    CAN_OBJ obj;
    for(int i=0;i<8&&i<mslist.length();i++){
        obj.Data[i]=QstringToInt(mslist[i]);
    }
    ULONG nums=Transmit(settings.devicetype,settings.deviceid,settings.canid,&obj,1);
    for(ULONG i=0;i<nums;i++){
        obj.ID=index+i;
        emit writeToTable(index+i,objs[i],QDateTime::currentDateTime().toString("yyyy_MM_dd hh:mm:ss:zzz"),-1);
    }
    index+=nums;
}

uint frameDisplay::QstringToInt(QString str){
    QByteArray publishPlotocal=str.toLatin1();
    QByteArray tempByteHigh,tempBytelow;
    tempByteHigh[0]=publishPlotocal.at(0);
    if(str.length()==1){
        return tempByteHigh.toInt(NULL,16);
    }
    tempBytelow[0]=publishPlotocal.at(1);
    return tempByteHigh.toInt(NULL,16)*16+tempBytelow.toInt(NULL,16);
}
void frameDisplay::on_spinBox_valueChanged(int arg1){
    index=arg1;
}
