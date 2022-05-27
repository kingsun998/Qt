#include "settings.h"
#include "qfont.h"
class settings settings;

settings::settings()
{
    firstChartLineNum=4;
    secondChartLineNum=3;
    pointsnum=20;
    defaultXlen=40;
    defaultXtickCount=7;
    defaultYtickCount=7;
    //图表的名称及颜色
    splinePen={Qt::red,Qt::green,Qt::blue,Qt::cyan,Qt::darkMagenta,Qt::gray};

    penweight=2;
    maxpoints=200;

    boterate=250;

    deviceid=0;
    canid=0;
    devicetype=3;

    Y_min=-10;
    Y_max=400;
    //baseinfo
    TestMode=false;

    //chartdisplay
    pInitConfig.AccCode=0;
    pInitConfig.AccMask=0xffffff;
    pInitConfig.Filter=0;
    pInitConfig.Timing0=0x01;
    pInitConfig.Timing1=0x1c;
    errorCode_TC={{0,"Pass"},{1,"TC Short to Vbat"},{2,"TC Short to Gnd"},{5,"TC Open ckt"},
               {6,"TC ASIC"},{11,"TC CJ"},{13,"TC ECU"},{16,"Not complete"}};
    errorCode_CJ={{0,"Pass"},{1,"CJ Short ckt"},{4,"CJ Open ckt"},{6,"CJ ASIC"},
               {14,"CJ Plausibility"},{16,"Not complete"}};
    errorCode_ECU={{0,"Pass"},{8,"ECU μC EEP"},{9,"ECU ASIC EEP"},{10,"ECU High Volt"},
               {11,"ECU Low Volt"},{15,"ECU Overtemp"},{16,"Not complete"}};


    CompanyName.append("A");
    CompanyName.append("B");

    Bt_temperature=25;
    Tp_temperature=400*0.63;

    saveInterval_minus=1;

    CompanyType=0;

    caninit=false;
    //sendmessages
    sendMessageInterval=500;
    canindex=1;

    //stander
    testColor={{0,Qt::red},{1,Qt::green},{-1,Qt::gray}};

    //framedisplay
    maxrowcount=100;
    saveTable_Interval=saveInterval_minus;

    send_frame_interval=1000;
    send_frame_type={{0,"数据帧"},{1,"远程帧"}};
    send_frame_format={{0,"标准帧"},{1,"扩展帧"}};
    send_frame_id=0;
    send_frame_nums=0;
    id_auto_increase=false;
    unsigned char ascii[16]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};
    memcpy(ox_ascii,ascii,16);

    //dbcontroller
    mergeFile_interval=30;
    allow_show=true;
    xlsxMaxline=65000;
    dirnames={"charts","frames","testsChannel1","testsChannel2","tempcharts","tempframes","temptestsChannel1","temptestsChannel2"};

    //testdisplay
    company_datenames={"T1","T2","T3","T4","CJ","μC","V1","V2","V3","V4"};
    company_usedate_names={"T1","T2","T3","T4","CJ","μC"};
    company_col_title={"val","status","time","hightempTest","precisionTest"};
    HightempThreshold=5;
    //usbcanunion
    timer_interval=200;
    //hightempTest,precisionTest  status
    Test_status={{0,"ERROR"},{1,"PASS"},{2,"NO TEST"},{3,"..."}};
    Test_status_color={{0,Qt::red},{1,Qt::green},{2,Qt::gray}};
    //mergefile  dbcontroller
    mergesize=4;
    //show_clock
    show_clock=3;


//    mqtt_ip_address="192.168.1.27";
    mqtt_ip_address="127.0.0.1";
    mqtt_port=1883;
    topic="product_param_topic";
    qos=1;
    username="znpw";
    password="ZNJC_8888";

    //test
    baurd=9600;
    stopbit=1;
    check=0;
    datalen=8;
    testTableMaxRowNum=16-1;
    eightChannelFrameMes="";

    //globel
    pushbutton_font=new QFont();
    pushbutton_font->setPointSize(12);
    pushbutton_font->setWeight(QFont::Black);

    showlabel_font=new QFont();
    showlabel_font->setPointSize(12);
    showlabel_font->setWeight(QFont::Black);

    //deviceassembly
    probeName={"T1","T2","T3","T4","CJ","μC"};
}

void settings::setbote(){
    switch (boterate) {
    case 1000:
        this->sjw=1;
        this->bs1=15;
        this->bs2=5;
        this->brp=2;
    break;
    case 800:
        this->sjw=1;
        this->bs1=2;
        this->bs2=1;
        this->brp=13;
    break;
    case 666:
        this->sjw=1;
        this->bs1=16;
        this->bs2=4;
        this->brp=3;
    break;
    case 500:
        this->sjw=1;
        this->bs1=16;
        this->bs2=4;
        this->brp=4;
    break;
    case 400:
        this->sjw=1;
        this->bs1=12;
        this->bs2=2;
        this->brp=7;
    break;
    case 250:
        this->sjw=1;
        this->bs1=6;
        this->bs2=1;
        this->brp=21;
    break;
    case 125:
        this->sjw=1;
        this->bs1=6;
        this->bs2=1;
        this->brp=42;
    break;
    case 100:
        this->sjw=1;
        this->bs1=15;
        this->bs2=4;
        this->brp=21;
    break;
    default:
        break;
    }
}


void showMessage(QString str,bool flag){
    if(settings.allow_show){
        if(flag){
           qDebug()<<str;
        }
    }

}
