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
    ifsending=false;
    file=new std::ifstream();
    currentlength=0;
    emit updateProcess(0);
}


SendMessages::~SendMessages(){

}

int SendMessages::init(){

    //初始化硬件设备
    if(!initDevice(DevHandle)){
        qDebug()<<"1";
    }
    //输出硬件设备信息
//    if(!getDeviceInfo(DevHandle[0],DevInfo)){
//        qDebug()<<"2";
//    }
    //初始化配置CAN
    if(!initCan(DevHandle[0])){
        qDebug()<<"3";
    }
    //配置过滤器，必须配置，否则可能无法收到数据
    if(!initFilter(DevHandle[0])){
            qDebug()<<"4";
    }
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

int SendMessages::initCan(int DevAddress){
    CAN_INIT_CONFIG CANConfig;
    CANConfig.CAN_Mode = 0;//正常模式
//    CANConfig.CAN_Mode = 1;//环回模式
    CANConfig.CAN_ABOM = 0;//禁止自动离线
    CANConfig.CAN_NART = 1;//禁止报文重传
    CANConfig.CAN_RFLM = 0;//FIFO满之后覆盖旧报文
    CANConfig.CAN_TXFP = 1;//发送请求决定发送顺序
    //配置波特率,波特率 = 100M/(BRP*(SJW+BS1+BS2))
    //可以在文档中查找
    CANConfig.CAN_BRP = settings.brp;
    CANConfig.CAN_BS1 = settings.bs1;
    CANConfig.CAN_BS2 = settings.bs2;
    CANConfig.CAN_SJW = settings.sjw;
    //
    int ret = CAN_Init(DevAddress,settings.canindex,&CANConfig);
//    int ret = CAN_Init(DevAddress,CANIndex,&CANConfig);
    if(ret != CAN_SUCCESS){
        setcolor(ui->label_9,Qt::red,"初始化Can失败");
        return 0;
    }else{
        setcolor(ui->label_9,Qt::green,"初始化Can成功");
    }
    return 1;
}

int SendMessages::initFilter(int DevAddress){
    CAN_FILTER_CONFIG CANFilter;
    CANFilter.Enable = 1;
    CANFilter.ExtFrame = 0;
    CANFilter.FilterIndex = 0;
    CANFilter.FilterMode = 0;
    CANFilter.MASK_IDE = 0;
    CANFilter.MASK_RTR = 0;
    CANFilter.MASK_Std_Ext = 0;
    int ret = CAN_Filter_Init(DevAddress,settings.canindex,&CANFilter);
    if(ret != CAN_SUCCESS){
        setcolor(ui->label_7,Qt::red,"初始化滤波失败");
        return 0;
    }else{
        setcolor(ui->label_7,Qt::green,"初始化滤波成功");
    }
    return 1;
}


void SendMessages::handleTimeOut(){
    if(!file->eof()){

        int DevAddress=DevHandle[0];
        CAN_MSG msg[256];
        uint nums=0;
        while (!file->eof()&&nums<256){
            for (int i=0;i<8;i++) {
                *file>>msg[nums].Data[i];
            }
            msg[nums].DataLen=8;
            msg[nums].ExternFlag=0;
            msg[nums].RemoteFlag=0;
            msg[nums].ID=baseID+nums;
            nums++;
        }
        currentlength+=nums*8;
        int SendedNum = CAN_SendMsg(DevAddress,settings.canindex,msg,nums);
//        qDebug()<<file->eof()<<"  "<<nums;
//        qDebug()<<"c "<<currentlength<<"t "<<totallength;
        emit updateProcess(int(currentlength*100.0/totallength));
        if(SendedNum == CAN_SUCCESS){
            setcolor(ui->label,Qt::green,"发送成功");
        }else{
            setcolor(ui->label,Qt::red,"发送失败");
        }
    }else{
        //关闭文件，关闭计时器，发送监视器设置为false
        file->close();
        timer->stop();
        ifsending=false;
        emit updateProcess(100);
        QMessageBox::information(this,"提示","传输完成",QMessageBox::Warning);
        ui->pushButton_2->setText("发送");
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

        qDebug()<<filepath;
        //void open(const char* filename,int mode,int access);

        //设置总长度和当前长度
        FILE* f = fopen(ui->label_6->text().toLocal8Bit().data(), "rb");
        totallength = filelength(fileno(f));
        fclose(f);
        currentlength=0;
        file->open(filepath.toStdString(), std::ios::in|std::ios::binary);

     }
}

//选择文件
void SendMessages::on_pushButton_2_clicked()
{
    qDebug()<<ifsending;
    if(ifsending==true){
        ui->pushButton_2->setText("暂停");
        timer->stop();
    }
    else{
        if(!file->is_open()){
            QMessageBox::information(this,"提示","请选择文件",QMessageBox::Warning);
            return;
        }
        //初始化当前长度
        currentlength=0;
        ui->pushButton_2->setText("发送");
        timer->start();
    }
    ifsending=!ifsending;
}

//初始ID
void SendMessages::on_lineEdit_2_editingFinished()
{
    baseID=ui->lineEdit_2->text().toInt();
}

//重置
void SendMessages::on_pushButton_3_clicked()
{
    init();
}
