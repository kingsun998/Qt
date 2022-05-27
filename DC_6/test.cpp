#include "test.h"
#include "ui_test.h"
#include <QDebug>
#include <QMessageBox>
#include <QVariant>
#include <QTime>
#include <settings.h>



Test::Test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Test)
{
    ui->setupUi(this);


    /* 行颜色交替显示 */
    ui->tableWidget->setAlternatingRowColors(true);
    /* 不允许在图形界面修改内容 */
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /* 设置列数 */
    QStringList stringlists;
    stringlists<<"测试时间"<<"传感器ID"<<"电压值"<<"电阻值"<<"测试状态"<<"测试花费时间";
    ui->tableWidget->setColumnCount(stringlists.size());
    ui->tableWidget->setHorizontalHeaderLabels(stringlists);
    ui->tableWidget->setMinimumWidth(400);
    ui->tableWidget->setColumnWidth(1,400);

    QStringList stringlists2;
    stringlists2<<"测试时间"<<"传感器ID"<<"电阻值"<<"电压值"<<"测试状态"<<"测试花费时间";
    ui->tableWidget_2->setColumnCount(stringlists2.size());
    ui->tableWidget_2->setAlternatingRowColors(true);
//    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setHorizontalHeaderLabels(stringlists2);
    ui->tableWidget_2->setMinimumWidth(400);
    ui->tableWidget_2->setColumnWidth(1,400);


    ui->pushButton_3->setEnabled(false);
    QFont font;
    font.setPointSize(13);
    ui->tableWidget->setFont(font);
    ui->tableWidget_2->setFont(font);
    openport=false;

    connect(&wzserialport_single,&WZSerialPort::ReceiveSinglePortMes,this,&Test::LoadSinglePortMes);
    connect(&wzserialport_eight,&WZSerialPort::ReceiveEightPortMes,this,&Test::LoadEightPortMes);


    wzserialport_single.setPortStruct(&singleportstruct);
    wzserialport_eight.setPortStruct(&eightportstruct);

    scangun=nullptr;

    qDebug()<<"main thread:"<<QThread::currentThreadId();

    rowcount_eight=0;
    rowcount_single=0;
    water_index=0;

    pushbutton_list.append(ui->pushButton);
    pushbutton_list.append(ui->pushButton_2);
    pushbutton_list.append(ui->pushButton_3);
    pushbutton_list.append(ui->pushButton_4);
    pushbutton_list.append(ui->pushButton_5);
    pushbutton_list.append(ui->pushButton_6);
    pushbutton_list.append(ui->pushButton_7);
    pushbutton_list.append(ui->pushButton_8);
    pushbutton_list.append(ui->pushButton_9);
    for (int i=0;i<pushbutton_list.size();i++) {
        pushbutton_list[i]->setFont(*settings.pushbutton_font);
        pushbutton_list[i]->setMinimumSize(130,40);
    }
    QVector<QComboBox*> combobox_list;
    combobox_list.append(ui->comboBox);
    combobox_list.append(ui->comboBox_3);
    for (int i=0;i<combobox_list.size();i++) {
        combobox_list[i]->setFont(*settings.pushbutton_font);
        combobox_list[i]->setMinimumSize(130,40);
    }
}

Test::~Test()
{
    delete ui;
}
//void Test::tabChange(int index){
//    if(index==0){
//        wzserialport.setPortStruct(portstruct_map["single_channel"]);
//    }else{
//        wzserialport.setPortStruct(portstruct_map["eight_channel"]);
//    }
//}

/* *
 * 沁水绝缘测试  1通
 * */

//扫描二维码
void Test::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    water_index=0;
    int currentpage=ui->tabWidget->currentIndex();
    QString name=ui->tabWidget->tabText(currentpage);
    scangun=new ScanGun(this,&waterinsulation);
    connect(scangun,&ScanGun::closeTheDialog,this,&Test::scangunClose);
    scangun->setAttribute(Qt::WA_DeleteOnClose);
    scangun->show();
}
//发送数据  单通
void Test::on_pushButton_2_clicked()
{
    if(!waterinsulation.size()){
        QMessageBox::critical(nullptr, QLatin1String("Error"),
                              "未扫描");
        return;
    }
    SinglePortStruct *pt=&singleportstruct;
    MqttDataStruct data;
    data.AddTestName(3);
    int size=waterinsulation.size();
    qDebug()<<rowcount_single;
    for (int i=0;i<size;i++) {
        QString sensorName=QString::number(i);
        int rowindex=rowcount_single-i-1;
        data.AddObject(sensorName,"sensorId",ui->tableWidget->item(rowindex,1)->text() );
        data.AddObject(sensorName,"voltage",ui->tableWidget->item(rowindex,2)->text());
        data.AddObject(sensorName,"resistance",ui->tableWidget->item(rowindex,3)->text());
        data.AddObject(sensorName,"testState",ui->tableWidget->item(rowindex,4)->text());
        data.AddObject(sensorName,"cost_time",ui->tableWidget->item(rowindex,5)->text());
        data.AddObject(sensorName,"test_time",ui->tableWidget->item(rowindex,0)->text());
    }

    mqtt.publish_Mes(data);

}
//加载数据
void Test::on_pushButton_3_clicked()
{
    //AA  FA  BF
    ui->pushButton_3->setEnabled(false);
    emit wzserialport_single.send_receive();
}
// 扫描串口
void Test::on_pushButton_5_clicked()
{
    ui->comboBox->clear();
    QStringList &&comname= wzserialport_single.get_available_list();
    ui->comboBox->addItems(comname);
}

// 沁水绝缘性 打开串口
void Test::on_pushButton_6_clicked(){
    //串口名称，波特率，奇偶校验，数据位，停止位，同步方式
    //0为异步，1为同步
    if(!wzserialport_single.open(ui->comboBox->currentText().toStdString().c_str(),2400,0,8,1,1)){
        QMessageBox::critical(nullptr,"错误","打开串口失败");
        return;
    }
    ui->pushButton_3->setEnabled(true);

}
//改变串口
void Test::on_comboBox_currentIndexChanged(int index)
{
    ui->pushButton_3->setEnabled(false);
}
//加载单通信息
void Test::LoadSinglePortMes(){
    qDebug()<<"LoadSinglePortMes";
    QString sensorid;
    if(water_index<waterinsulation.size()){
        sensorid=waterinsulation[water_index++];
    }else{
        sensorid=" ";
    }
    SinglePortStruct *pt=&singleportstruct;

    ui->tableWidget->insertRow(rowcount_single);
    pt->testtime=QTime::currentTime().toString();


    QTableWidgetItem *current_time=new QTableWidgetItem(pt->testtime);
    current_time->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setItem(rowcount_single,0,current_time);

    QTableWidgetItem *sensor_id=new QTableWidgetItem(sensorid);
    sensor_id->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setItem(rowcount_single,1,sensor_id);

    QTableWidgetItem *voltage=new QTableWidgetItem(QString::fromStdString(pt->voltage));
    voltage->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setItem(rowcount_single,2,voltage);

    QTableWidgetItem *resistance=new QTableWidgetItem(QString::fromStdString(pt->resistance));
    resistance->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setItem(rowcount_single,3,resistance);

    QTableWidgetItem *testState=new QTableWidgetItem(QString::fromStdString(pt->testState));
    testState->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setItem(rowcount_single,4,testState);

    QTableWidgetItem *time=new QTableWidgetItem(QString::fromStdString(pt->time));
    time->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setItem(rowcount_single,5,time);
    if(rowcount_single>settings.testTableMaxRowNum){
        ui->tableWidget->removeRow(0);
    }else{
        rowcount_single+=1;
    }
    ui->pushButton_3->setEnabled(true);
}

void Test::scangunClose(){
    disconnect(scangun,&ScanGun::closeTheDialog,this,&Test::scangunClose);
    ui->pushButton->setEnabled(true);
    for (int i=0;i<waterinsulation.size();i++) {
        qDebug()<<waterinsulation[i];
    }
    qDebug()<<"water size:"<<waterinsulation.size();
}
/* *
 * 高温绝缘测试  8通
 * */

// 打开串口
void Test::on_pushButton_9_clicked()
{
    if(!wzserialport_eight.open(ui->comboBox_3->currentText().toStdString().c_str(),settings.baurd,settings.check,settings.datalen,settings.stopbit,1)){
        QMessageBox::critical(nullptr,"错误","打开串口失败");
        return;
    }
}

//// 扫码
//void Test::on_pushButton_4_clicked()
//{
//    int currentpage=ui->tabWidget->currentIndex();
//    QString name=ui->tabWidget->tabText(currentpage);
//    ui->pushButton_4->setEnabled(false);
//    scangun2=new ScanGun(this,&settings.eightChannelSnCode);
//    connect(scangun2,&ScanGun::closeTheDialog,this,&Test::scangun2Close);
//    scangun2->setAttribute(Qt::WA_DeleteOnClose);
//    scangun2->show();
//}
// 发送数据  8通
void Test::on_pushButton_8_clicked()
{

    EightPortStruct *pt=&eightportstruct;
    MqttDataStruct data;
    data.AddTestName(1);
    int size=settings.eightChannelSnCode.size();
    for (int i=0;i<size;i++) {
        QString name="sensor_"+QString::number(i);
        data.AddObject(name,"sensorId",settings.eightChannelSnCode[i]);
        data.AddObject(name,"resistance",QString::fromStdString(pt->resistance[i]));
        data.AddObject(name,"voltage",QString::fromStdString(pt->voltage[i]));
        data.AddObject(name,"testState",pt->testState[i]?"Pass":"Error");
        data.AddObject(name,"cost_time",QString::number(pt->time));
        data.AddObject(name,"test_time",pt->testtime);
    }
    mqtt.publish_Mes(data);
}
// 扫描串口2
void Test::on_pushButton_7_clicked()
{
    ui->comboBox_3->clear();
    QStringList &&comname= wzserialport_eight.get_available_list();
    ui->comboBox_3->addItems(comname);
}
//更改串口
void Test::on_comboBox_3_currentIndexChanged(int index)
{
//        ui->pushButton_9->setEnabled(false);
}
// 加载八通信息
void Test::LoadEightPortMes(){

    EightPortStruct *pt=&eightportstruct;

    pt->testtime=QTime::currentTime().toString();
    int actualSize=settings.eightChannelSnCode.size();
    for (uint i=0;i<8;i++) {
        QString senserSN=i<actualSize?settings.eightChannelSnCode[i]:" ";
        ui->tableWidget_2->insertRow(rowcount_eight);

        QTableWidgetItem *current_time=new QTableWidgetItem(pt->testtime);
        current_time->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget_2->setItem(rowcount_eight,0,current_time);

        QTableWidgetItem *sensor_id=new QTableWidgetItem(senserSN);
        sensor_id->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget_2->setItem(rowcount_eight,1,sensor_id);

        QTableWidgetItem *resistance=new QTableWidgetItem(QString::fromStdString(pt->resistance[i]));
        resistance->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget_2->setItem(rowcount_eight,2,resistance);

        QTableWidgetItem *voltage=new QTableWidgetItem(QString::fromStdString(pt->voltage[i]));
        voltage->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget_2->setItem(rowcount_eight,3,voltage);

        QTableWidgetItem *status=new QTableWidgetItem(pt->testState[i]?"Pass":"Error");
        status->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget_2->setItem(rowcount_eight,4,status);

        QTableWidgetItem *time=new QTableWidgetItem(QString::number(pt->time));
        time->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget_2->setItem(rowcount_eight,5,time);

        if(rowcount_eight>settings.testTableMaxRowNum){
            ui->tableWidget_2->removeRow(0);
        }else{
            rowcount_eight+=1;
        }
    }
//    settings.eightChannelSnCode.clear();

}

//void  Test::scangun2Close(){
//    disconnect(scangun2,&ScanGun::closeTheDialog,this,&Test::scangun2Close);
//    ui->pushButton_4->setEnabled(true);
//    qDebug()<<"hightmp size:"<<settings.eightChannelSnCode.size();
//}

void Test::on_pushButton_4_clicked()
{
    if(water_index>0){
        water_index-=1;
        ui->tableWidget->removeRow(rowcount_single-1);
        rowcount_single-=1;
    }else{
        QMessageBox::information(this,"错误","不能再撤销!");
    }

}
