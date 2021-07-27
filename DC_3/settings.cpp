#include "settings.h"

settings::settings()
{
    firstChartLineNum=4;
    secondChartLineNum=3;
    pointsnum=20;
    defaultXlen=40;
    defaultXtickCount=11;
    defaultYtickCount=11;
    splineName={"A1DOC intake","A1DOC outlet","A1SCR intake","A1SCR outlet","AT1IG2 intake","CJ","uC"};
    splinePen={Qt::red,Qt::yellow,Qt::green,Qt::blue,Qt::cyan,Qt::darkMagenta,Qt::gray};
    penweight=2;
    maxpoints=200;

    maxrowcount=20;

    Bt_temperature=27;
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

    saveInterval_minus=10;

    taskCode={{"SaveChart",0},{"SaveTable",1}};

    basepath="../savefiles/";

    lineNums=7;

    sendMessageInterval=10;
}
