#include "settings.h"


settings::settings()
{
    firstChartLineNum=4;
    secondChartLineNum=3;
    pointsnum=20;
    defaultXlen=40;
    defaultXtickCount=11;
    defaultYtickCount=11;
    splineName={"A1DOC intake","A1DOC outlet","A1SCR intake","A1SCR outlet",
                "AT1IG2 intake","CJ","uC","V1","V2","V3","V4"};
    splinePen={Qt::red,Qt::yellow,Qt::green,Qt::blue,Qt::cyan,Qt::darkMagenta,Qt::gray};
    penweight=2;
    maxpoints=200;

    boterate=1000;

    maxrowcount=20;

    Bt_temperature=29;
    Tp_temperature=30;

    deviceid=0;
    canid=0;
    devicetype=3;

    pInitConfig.AccCode=0;
    pInitConfig.AccMask=0xffffff;
    pInitConfig.Filter=0;
    pInitConfig.Timing0=0x01;
    pInitConfig.Timing1=0x1c;

    Y_min=-10;
    Y_max=400;

    errorCode_TC={{0,"Pass"},{1,"TC Short to Vbat"},{2,"TC Short to Gnd"},{5,"TC Open ckt"},
               {6,"TC ASIC"},{11,"TC CJ"},{13,"TC ECU"},{16,"Not complete"}};
    errorCode_CJ={{0,"Pass"},{1,"CJ Short ckt"},{4,"CJ Open ckt"},{6,"CJ ASIC"},
               {14,"CJ Plausibility"},{16,"Not complete"}};
    errorCode_TC={{0,"Pass"},{8,"ECU μC EEP"},{9,"ECU ASIC EEP"},{10,"ECU High Volt"},
               {11,"ECU Low Volt"},{15,"ECU Overtemp"},{16,"Not complete"}};
    errorCode_vol={

    };

    saveInterval_minus=0.1;

    taskCode={{"SaveChart",0},{"SaveTable",1}};

    basepath="../savefiles/";

    lineNums=7;

    sendMessageInterval=100;

    canindex=1;

    CompanyType=0;

    totalnums=11;
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
