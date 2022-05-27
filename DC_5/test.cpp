#include "test.h"
#include "ui_test.h"
#include <QDebug>
#include <QMessageBox>
#include <QVariant>
#include <QTime>
#include <settings.h>
#include <dbcontroller.h>


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
    stringlists<<"测试时间"<<"传感器ID"<<"电压值"<<"电阻值"<<"测试状态"<<"测试花费时间(0.1s)";
    ui->tableWidget->setColumnCount(stringlists.size());
    ui->tableWidget->setHorizontalHeaderLabels(stringlists);
    ui->tableWidget->setMinimumWidth(400);
    ui->tableWidget->setColumnWidth(1,400);

    QStringList stringlists2;
    stringlists2<<"测试时间"<<"传感器ID"<<"电压值"<<"电阻值"<<"测试状态"<<"测试花费时间(0.1s)";
    ui->tableWidget_2->setColumnCount(stringlists2.size());
    ui->tableWidget_2->setAlternatingRowColors(true);

    ui->tableWidget_2->setHorizontalHeaderLabels(stringlists2);
    ui->tableWidget_2->setMinimumWidth(400);
    ui->tableWidget_2->setColumnWidth(1,400);


    ui->pushButton_3->setEnabled(false);
    ui->pushButton_10->setEnabled(false);

    QFont font;
    font.setPointSize(13);
    ui->tableWidget->setFont(font);
    ui->tableWidget_2->setFont(font);

    connect(&wzserialport_single,&WZSerialPort::ReceiveSinglePortMes,this,&Test::LoadSinglePortMes);
    connect(&wzserialport_eight,&WZSerialPort::ReceiveSinglePortMes,this,&Test::LoadEightPortMes);


    wzserialport_single.setPortStruct(&singleportstruct_water);
    wzserialport_eight.setPortStruct(&singleportstruct_hightemp);

    scangun=nullptr;

    qDebug()<<"main thread:"<<QThread::currentThreadId();

    rowcount_eight=0;
    rowcount_single=0;
    water_index=0;
    highTemp_index=0;

    pushbutton_list.append(ui->pushButton);
    pushbutton_list.append(ui->pushButton_3);
    pushbutton_list.append(ui->pushButton_4);
    pushbutton_list.append(ui->pushButton_5);
    pushbutton_list.append(ui->pushButton_6);
    pushbutton_list.append(ui->pushButton_7);
    pushbutton_list.append(ui->pushButton_9);
    pushbutton_list.append(ui->pushButton_10);
    pushbutton_list.append(ui->pushButton_11);
    pushbutton_list.append(ui->pushButton_12);
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

    timer.setInterval(settings.saveInterval_minus*60*1000);
    connect(&timer,&QTimer::timeout,this,&Test::handleTimeOut);
    timer.start();
}

Test::~Test()
{
    delete ui;
}

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

//加载数据
void Test::on_pushButton_3_clicked()
{
    if(settings.TestMode){
//        开启测试模式
        std::string  current_time=QTime::currentTime().toString().toStdString();
        singleportstruct_water.voltage=current_time;
        singleportstruct_water.resistance=current_time;
        singleportstruct_water.electric=current_time;
        singleportstruct_water.testState=current_time;
        singleportstruct_water.time=current_time;
        emit LoadSinglePortMes();
    }else{
        //AA  FA  BF
        ui->pushButton_3->setEnabled(false);
        emit wzserialport_single.send_receive();
    }
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
        sensorid=waterinsulation[water_index];
    }else{
        sensorid=" ";
    }
    water_index++;
    SinglePortStruct *pt=&singleportstruct_water;

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

    waterinsulationUnIon.push_back(singleportstruct_water);
    waterSensorIdUnion.push_back(sensorid);
}

void Test::scangunClose(){

    disconnect(scangun,&ScanGun::closeTheDialog,this,&Test::scangunClose);
    ui->pushButton->setEnabled(true);

}

// 撤销上一条数据
void Test::on_pushButton_4_clicked()
{
    if(water_index>0){
        water_index-=1;
        ui->tableWidget->removeRow(rowcount_single-1);
        waterinsulationUnIon.pop_back();
        waterSensorIdUnion.pop_back();
        rowcount_single-=1;
    }else{
        QMessageBox::information(this,"错误","不能再撤销!");
    }

}
/* *
 * 高温绝缘测试  8通
 * */

// 打开串口
void Test::on_pushButton_9_clicked()
{
     if(!wzserialport_eight.open(ui->comboBox_3->currentText().toStdString().c_str(),2400,0,8,1,1)){
         QMessageBox::critical(nullptr,"错误","打开串口失败");
         return;
     }
     ui->pushButton_10->setEnabled(true);
}

// 扫码
void Test::on_pushButton_11_clicked()
{
    ui->pushButton_11->setEnabled(false);
    highTemp_index=0;
    int currentpage=ui->tabWidget->currentIndex();
    QString name=ui->tabWidget->tabText(currentpage);
    scangun2=new ScanGun(this,&highTempInsulation);
    connect(scangun2,&ScanGun::closeTheDialog,this,&Test::scangun2Close);
    scangun2->setAttribute(Qt::WA_DeleteOnClose);
    scangun2->show();
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
    qDebug()<<"Load HighTempInsulation PortMes";
    QString sensorid;
    if(highTemp_index<highTempInsulation.size()){
        sensorid=highTempInsulation[highTemp_index];
    }else{
        sensorid=" ";
    }
    highTemp_index++;
    SinglePortStruct *pt=&singleportstruct_hightemp;

    ui->tableWidget_2->insertRow(rowcount_eight);
    pt->testtime=QTime::currentTime().toString();



    QTableWidgetItem *current_time=new QTableWidgetItem(pt->testtime);
    current_time->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget_2->setItem(rowcount_eight,0,current_time);

    QTableWidgetItem *sensor_id=new QTableWidgetItem(sensorid);
    sensor_id->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget_2->setItem(rowcount_eight,1,sensor_id);

    QTableWidgetItem *voltage=new QTableWidgetItem(QString::fromStdString(pt->voltage));
    voltage->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget_2->setItem(rowcount_eight,2,voltage);

    QTableWidgetItem *resistance=new QTableWidgetItem(QString::fromStdString(pt->resistance));
    resistance->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget_2->setItem(rowcount_eight,3,resistance);

    QTableWidgetItem *testState=new QTableWidgetItem(QString::fromStdString(pt->testState));
    testState->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget_2->setItem(rowcount_eight,4,testState);

    QTableWidgetItem *time=new QTableWidgetItem(QString::fromStdString(pt->time));
    time->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget_2->setItem(rowcount_eight,5,time);

    if(rowcount_eight>settings.testTableMaxRowNum){
        ui->tableWidget_2->removeRow(0);
    }else{
        rowcount_eight+=1;
    }
    ui->pushButton_10->setEnabled(true);

    highTempInsulationUnIon.push_back(singleportstruct_hightemp);
    highSensorIdUnion.push_back(sensorid);

}

// 加载数据
void Test::on_pushButton_10_clicked()
{
    if(settings.TestMode){
//        开启测试模式
        std::string  current_time=QTime::currentTime().toString().toStdString();
        singleportstruct_hightemp.voltage=current_time;
        singleportstruct_hightemp.resistance=current_time;
        singleportstruct_hightemp.electric=current_time;
        singleportstruct_hightemp.testState=current_time;
        singleportstruct_hightemp.time=current_time;
        emit LoadEightPortMes();
    }else{
        //AA  FA  BF
        ui->pushButton_10->setEnabled(false);
        emit wzserialport_eight.send_receive();
    }
}

// 撤销上一条
void Test::on_pushButton_12_clicked()
{
    if(highTemp_index>0||highTempInsulationUnIon.size()){
        if(highTemp_index>0)
            highTemp_index-=1;
        ui->tableWidget_2->removeRow(rowcount_eight-1);
        highTempInsulationUnIon.pop_back();
        highSensorIdUnion.pop_back();
        rowcount_eight-=1;
    }else{
        QMessageBox::information(this,"错误","不能再撤销!");
    }

}
void Test::scangun2Close(){
    qDebug()<<"scangun2close";
    disconnect(scangun2,&ScanGun::closeTheDialog,this,&Test::scangun2Close);
    ui->pushButton_11->setEnabled(true);

}

void Test::handleTimeOut(){
//    dbcontroller.addObject();
    //swap 速度快，且清空
    dbSinglePortTest *obj=new dbSinglePortTest(waterinsulationUnIon,waterSensorIdUnion,"water");
    dbcontroller.addObject(obj);
    obj=new dbSinglePortTest(highTempInsulationUnIon,highSensorIdUnion,"hightemp");
    dbcontroller.addObject(obj);
}
