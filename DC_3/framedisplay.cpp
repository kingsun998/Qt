#include "framedispaly.h"
#include "ui_framedisplay.h"
#include "dbservice.h"
#include <memory>
#include <windows.h>
#include <mutex>

frameDisplay::frameDisplay(QWidget *parent) : QWidget(parent),
    ui(new Ui::framedisplay_ui)
{
    ui->setupUi(this);

    //以前使用的是tabwidget
    //这里使用的是tabview，而这里使用model来适配tabview

    ui->tableWidget->setColumnCount(8);
    /* 设置列宽在可视界面自适应宽度 */
//    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
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

    saveInterval_miseconds=settings.saveChart_Interval*60*1000;
    timeClick=GetTickCount();

    send_frame_type=settings.send_frame_type[0];
    send_frame_format=settings.send_frame_format[0];
    send_frame_id=0;
    send_frame_interval=1000;
    send_frame_nums=1;

    //串口操作
    comprocess=new ComProcess(this);

    parameter.widget=this;
    parameter.dequein=100;
    parameter.dequeout=100;
    parameter.datalength=20;
    parameter.initDcbstr(9600,'N',8,1);
    comprocess->init(parameter);

    connect(comprocess->getService(),&ComService::receiveFromCom,this,&frameDisplay::receiveMesFromCom);
    connect(this,&frameDisplay::sendMesToCom,comprocess->getService(),&ComService::sendMessage);
    connect(comprocess->getService(),&ComService::sendCallBack,this,&frameDisplay::dispaly_comResult);
    switchReceive=false;
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

void frameDisplay::dispaly_comResult(QString mes){
    qDebug()<<"发送成功  "<<mes;
}
//接受数据开关
void frameDisplay::on_pushButton_6_clicked(){
    if(switchReceive==false)
        comprocess->startServiceTimer();
    else
        comprocess->stopServiceTimer();
    switchReceive=!switchReceive;
}
//发送数据
void frameDisplay::on_pushButton_5_clicked(){
    emit sendMesToCom(ui->lineEdit_2->text());
}



void frameDisplay::getMessage(int mx,CAN_OBJ obj,QString datetime,int companycode){
    //保存信息
    if(!dispaly_ifreceive){
        return;
    }
    //判断是否超过了预设时间
    if(GetTickCount()-timeClick>saveInterval_miseconds){
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
    CompanyName.push_back(settings.CompanyName[companycode]);
    FrameID.push_back(obj.ID);
    FrameLen.push_back(obj.DataLen);
    QString data_info = QString("%1").arg(obj.Data[0], 2, 16, QLatin1Char('0')).toUpper();
    for(int data_long=1;data_long<8;data_long++)
    {
        data_info = data_info + " "+QString("%1").arg(obj.Data[data_long], 2, 16, QLatin1Char('0')).toUpper();
    }
    FrameContent.push_back(data_info);
    Data.push_back(datetime);
    //显示
   if(dispaly_allowshow==false){
       return;
   }
   ui->tableWidget->insertRow(rowcount);

   QVector<QTableWidgetItem *> vec;

   QTableWidgetItem *lineid=new QTableWidgetItem(QString::number(mx,10));
   lineid->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,0,lineid);
   vec.push_back(lineid);

   QTableWidgetItem *devicetype=new QTableWidgetItem(QString::number(settings.devicetype));
   devicetype->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,1,devicetype);
   vec.push_back(devicetype);

   QTableWidgetItem *messageExternItem=new QTableWidgetItem(messageExtern);
   messageExternItem->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,2,messageExternItem);
   vec.push_back(messageExternItem);

   QTableWidgetItem *frametype=new QTableWidgetItem("远程帧|本地帧");
   frametype->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,3,frametype);
   vec.push_back(frametype);

   QTableWidgetItem *frameID=new QTableWidgetItem(QString::number(obj.ID,16));
   frameID->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,4,frameID);
   vec.push_back(frameID);

   QTableWidgetItem *framelen=new QTableWidgetItem(QString::number(obj.DataLen,10));
   framelen->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,5,framelen);
   vec.push_back(framelen);

   //这里写 Data
   QTableWidgetItem *df=new QTableWidgetItem(data_info);
   df->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,6,df);
   vec.push_back(df);

   QTableWidgetItem *dt=new QTableWidgetItem(datetime);
   dt->setTextAlignment(Qt::AlignHCenter);
   ui->tableWidget->setItem(rowcount,7,dt);
   vec.push_back(dt);

   list.append(vec);
   if(rowcount>settings.maxrowcount){
       ui->tableWidget->removeRow(0);
       int size=list.begin()->size();
       for(auto i=0;i<size;i++){
            delete (*list.begin())[i];
       }
       list.removeFirst();
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

Q_DECLARE_METATYPE(QVector<uint>);
void frameDisplay::on_pushButton_clicked(){
    //保存期间关闭接受
//    qDebug()<<&settings;
    dispaly_ifreceive=false;
    //加一层锁
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    QVector<QString> newFrameType;
    QVector<QString> newCompanyName;
    QVector<uint> newFrameID;
    QVector<uint> newFrameLen;
    QVector<QString> newFrameContent;
    QVector<QString> newData;
    newFrameType.swap(FrameType);
    newCompanyName.swap(CompanyName);
    newFrameID.swap(FrameID);
    newFrameLen.swap(FrameLen);
    newFrameContent.swap(FrameContent);
    newData.swap(Data);
//    qDebug()<<newData.size();
//    for(int i=0;i<newData.size();i++){
//        qDebug()<<i<<"  "<<newData[i];
//    }
    QVariant type=QVariant::fromValue(newFrameType);
    QVariant name=QVariant::fromValue(newCompanyName);
    QVariant id=QVariant::fromValue(newFrameID);
    QVariant len=QVariant::fromValue(newFrameLen);
    QVariant content=QVariant::fromValue(newFrameContent);
    QVariant data=QVariant::fromValue(newData);

    emit db.saveTable(content,data,id,type,name,len,
                              QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
    timeClick=GetTickCount();
    dispaly_ifreceive=true;
}

//发送帧格式
void frameDisplay::on_comboBox_3_activated(int index){
    send_frame_format=settings.send_frame_format[index];
}
//发送帧种类
void frameDisplay::on_comboBox_4_activated(int index){
    send_frame_type=settings.send_frame_type[index];
}
//修改ID
void  frameDisplay::on_spinBox_4_valueChanged(int arg1){
    qDebug()<<"ID  "<<arg1;
    send_frame_id=arg1;
}
//修改发送间隔
void frameDisplay::on_spinBox_2_valueChanged(int arg1){
    send_frame_interval=arg1;
}
//修改发送数量
void frameDisplay::on_spinBox_3_valueChanged(int arg1){
    send_frame_nums=arg1;
}
//清楚计数
void frameDisplay::on_pushButton_3_clicked(){
    ui->label_16->setText(QString("0"));
    ui->label_37->setText(QString("0"));
    send_nums=0;
    receive_nums=0;
}
void frameDisplay::on_pushButton_7_clicked(){
    comprocess->init(parameter);
}
