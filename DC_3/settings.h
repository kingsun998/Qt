﻿#ifndef SETTINGS_H
#define SETTINGS_H

#include <windef.h>
#include <qvector.h>
#include <ECanVci.h>
#include <windows.h>
#include <map>
#include <QDebug>
#include <QMap>

extern class settings settings;

class settings
{
public:
    settings();
    void setbote();
public:
    //baseInfo
    DWORD devicetype;
    DWORD canid;
    DWORD deviceid;
    uint boterate;
    QVector<QString> varName;
    QVector<QPair<QString,int>> deviceInfo;
    QVector<QPair<QString,int>> caninfo;
    QVector<QPair<QString,int>> mes;
    uint sjw;
    uint bs1;
    uint bs2;
    uint brp;
    bool TestMode;

    //framedisplay
    int maxrowcount;
    double saveTable_Interval;
    std::map<int,QString> send_frame_type;
    std::map<int,QString> send_frame_format;
    uint send_frame_id;
    int send_frame_interval;
    uint send_frame_nums;
    bool id_auto_increase;
    unsigned char ox_ascii[16];

    //chartdisplay
    INIT_CONFIG pInitConfig;
    std::map<int,QString> errorCode_TC;
    std::map<int,QString> errorCode_CJ;
    std::map<int,QString> errorCode_ECU;
//    std::map<int,QString> errorCode_vol;
    double Bt_temperature;
    double Tp_temperature;
    double saveInterval_minus;
    int lineNums;
    int CompanyType;
    std::map<int,QString> CompanyName;
    int totalnums;
    bool caninit;

    //chart
    int firstChartLineNum;
    int secondChartLineNum;
    int pointsnum;
    int defaultXlen;
    int defaultXtickCount;
    int defaultYtickCount;
    QMap<int,QVector<QString>> splineName;
    QVector<Qt::GlobalColor> splinePen;
    int penweight;
    int maxpoints;
    double Y_min;
    double Y_max;

    //sendmessages
    uint sendMessageInterval;
    uint canindex;

    //stander
    QMap<bool,Qt::GlobalColor> testColor;

    //dbservice
    int xlsxMaxline;

    bool allow_show;

};

void showMessage(QString str,bool flag);
#endif // SETTINGS_H

