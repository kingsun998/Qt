#include "sendmessages.h"
#include "ui_sendmessages.h"

SendMessages::SendMessages(QWidget * parent):
    QWidget(parent),
    ui(new Ui::sendmessages_ui)
{
    ui->setupUi(this);
    CANIndex=0;
    timer=new QTimer();
    timer->setInterval(settings.sendMessageInterval);
    init();
    connect(timer,&QTimer::timeout,this,&SendMessages::handleTimeOut);
    connect(this,&SendMessages::updateProcess,ui->progressBar,&QProgressBar::setValue);
    baseID=0;
    emit updateProcess(0);
}


SendMessages::~SendMessages(){

}

int SendMessages::init(){

    //初始化硬件设备
    if(!initDevice(DevHandle)){
        qDebug()<<"1";
    }
    qDebug()<<"1_1";
    //输出硬件设备信息
    if(!getDeviceInfo(DevHandle[0],DevInfo)){
        qDebug()<<"2";
    }
    qDebug()<<"2_1";
    //初始化配置CAN
    if(!initCan(DevHandle[0],settings.canid)){
        qDebug()<<"3";
    }
    qDebug()<<"3_1";
    //配置过滤器，必须配置，否则可能无法收到数据
    if(!initFilter(DevHandle[0],settings.canid)){
            qDebug()<<"4";
    }
    qDebug()<<"4_1";
}

void setcolor(QLabel *widget,Qt::GlobalColor color,QString message){
    QPalette pa;
    pa.setColor(QPalette::WindowText,color);
    widget->setPalette(pa);
    widget->setText(message);
}

int SendMessages::initDevice(int* DevHandle){

    bool state;
    int ret;
    //扫描查找设备，并存在数组中
    ret = USB_ScanDevice(DevHandle);
    if(ret <= 0){
        setcolor(ui->label_11,Qt::red,"未扫描到设备");
        return 0;
    }
    //打开设备
    state = USB_OpenDevice(DevHandle[0]);
    if(!state){
        setcolor(ui->label_11,Qt::red,"打开设备失败");
        return 0;
    }
    setcolor(ui->label_11,Qt::green,"设备打开成功");
    return 1;
}

int SendMessages::getDeviceInfo(int DevAddress,DEVICE_INFO &DevInfo){
    char FunctionStr[256]={0};
    bool state;
    //获取固件信息
    state = DEV_GetDeviceInfo(DevAddress,&DevInfo,FunctionStr);
    if(!state){
        qDebug()<<("Get device infomation error!\n");
        return 0;
    }else{
        qDebug()<<"Firmware Info:\n";
        qDebug()<<"Firmware Name:%s\n"<<DevInfo.FirmwareName;
        qDebug()<<"Firmware Build Date:%s\n"<<DevInfo.BuildDate;
        qDebug()<<"Firmware Version:v%d.%d.%d\n"<<((DevInfo.FirmwareVersion>>24)&0xFF)<<((DevInfo.FirmwareVersion>>16)&0xFF)<<(DevInfo.FirmwareVersion&0xFFFF);
        qDebug()<<"Hardware Version:v%d.%d.%d\n"<<((DevInfo.HardwareVersion>>24)&0xFF)<<((DevInfo.HardwareVersion>>16)&0xFF)<<(DevInfo.HardwareVersion&0xFFFF);
        qDebug()<<"Firmware Functions:%s\n"<<FunctionStr;
    }
    return 1;
}

int SendMessages::initCan(int DevAddress,DWORD CANIndex){
    CAN_INIT_CONFIG CANConfig;
    CANConfig.CAN_Mode = 0;//正常模式
//    CANConfig.CAN_Mode = 1;//环回模式
    CANConfig.CAN_ABOM = 0;//禁止自动离线
    CANConfig.CAN_NART = 1;//禁止报文重传
    CANConfig.CAN_RFLM = 0;//FIFO满之后覆盖旧报文
    CANConfig.CAN_TXFP = 1;//发送请求决定发送顺序
    //配置波特率,波特率 = 100M/(BRP*(SJW+BS1+BS2))
    CANConfig.CAN_BRP = 2;
    CANConfig.CAN_BS1 = 15;
    CANConfig.CAN_BS2 = 5;
    CANConfig.CAN_SJW = 1;
    int ret = CAN_Init(DevAddress,CANIndex,&CANConfig);
    if(ret != CAN_SUCCESS){
        setcolor(ui->label_9,Qt::red,"初始化Can失败");
        return 0;
    }else{
        setcolor(ui->label_9,Qt::green,"初始化Can成功");
    }
    return 1;
}

int SendMessages::initFilter(int DevAddress,DWORD CANIndex){
    CAN_FILTER_CONFIG CANFilter;
    CANFilter.Enable = 1;
    CANFilter.ExtFrame = 0;
    CANFilter.FilterIndex = 0;
    CANFilter.FilterMode = 0;
    CANFilter.MASK_IDE = 0;
    CANFilter.MASK_RTR = 0;
    CANFilter.MASK_Std_Ext = 0;
    int ret = CAN_Filter_Init(DevAddress,CANIndex,&CANFilter);
    if(ret != CAN_SUCCESS){
        setcolor(ui->label_7,Qt::red,"初始化滤波失败");
        return 0;
    }else{
        setcolor(ui->label_7,Qt::green,"初始化滤波成功");
    }
    return 1;
}


void SendMessages::handleTimeOut(){
    if(!file.eof()){
        int DevAddress=DevHandle[0];
        CAN_MSG msg[256];
        uint nums=0;
        while (!file.eof()&&nums<256){
            for (int i=0;i<8;i++) {
                file>>msg[nums].Data[i];
            }
            msg[nums].DataLen=8;
            msg[nums].ExternFlag=0;
            msg[nums].RemoteFlag=0;
            msg[nums].ID=baseID+nums;
            nums++;
        }
        currentlength+=nums;
        emit updateProcess(int(currentlength*1.0/totallength));
        int SendedNum = CAN_SendMsg(DevAddress,CANIndex,msg,nums);
        if(SendedNum == CAN_SUCCESS){
            setcolor(ui->label,Qt::green,"发送成功");
        }else{
            setcolor(ui->label,Qt::red,"发送失败");
        }
    }else{
        file.close();
        timer->stop();
        QMessageBox::information(this,"提示","传输完成",QMessageBox::Warning);
        ui->pushButton_2->setText("开始");
    }
}


void SendMessages::on_pushButton_clicked()
{
    QFileDialog * fileDialog = new QFileDialog(this);
    //fileDialog->setDirectory("E:\\");
    fileDialog->setWindowTitle(QString::fromLocal8Bit("选择文件"));
    fileDialog->setNameFilter(QString::fromLocal8Bit("*.bin"));
    if ( fileDialog->exec() == QDialog::DialogCode::Accepted )
    {
        QString filepath = fileDialog->selectedFiles()[0];
        ui->label_6->setText(filepath);
        file.open(filepath.toStdString(), std::ios::in);
    }
}

void SendMessages::on_pushButton_2_toggled(bool checked)
{
    qDebug()<<checked;
    if(checked==true){
        ui->pushButton_2->setText("暂停");
        currentlength=0;
        int handle=open(ui->label_6->text().toLocal8Bit().data(), 0x0100); //open file for read
        totallength = filelength(handle);
        timer->stop();
    }
    else{
        ui->pushButton_2->setText("开始");
        timer->start();
    }
}

void SendMessages::on_lineEdit_2_editingFinished()
{
    baseID=ui->lineEdit_2->text().toInt();
}

void SendMessages::on_pushButton_3_clicked()
{
    qDebug()<<"点击";
    init();
}
