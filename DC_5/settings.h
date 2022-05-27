#ifndef SETTINGS_H
#define SETTINGS_H

#include <windef.h>
#include <qvector.h>
#include <ECanVci.h>
#include <windows.h>
#include <map>
#include <QDebug>
#include <QMap>
#include <mutex>
#include <settings.h>

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

    int CompanyType;
    QStringList CompanyName;
    bool caninit;

    //chart
    int firstChartLineNum;
    int secondChartLineNum;
    int pointsnum;
    int defaultXlen;
    int defaultXtickCount;
    int defaultYtickCount;

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

    //dbcontroller
    int xlsxMaxline;
    bool allow_show;
    QVector<QString> dirnames;
    double mergeFile_interval;

    //testdisplay
    int timer_interval;
    int HightempThreshold;
    QVector<QString> company_datenames;
    QVector<QString> company_usedate_names;
    QVector<QString> company_col_title;

    //hightempTest,precisionTest  status
    std::map<int,QString> Test_status;
    std::map<int,Qt::GlobalColor> Test_status_color;

    //mergefile
    int mergesize;
    //show_clock  显示间隔
    int show_clock;
    std::mutex mtx;

    //mqtt
    QString mqtt_ip_address;
    quint16 mqtt_port;
    QString topic;
    quint8 qos;
    QString username;
    QString password;
    //test
    int baurd;
    int stopbit;
    int check;
    int datalen;
    int testTableMaxRowNum;

    //test+testdisplay
    QVector<QString> eightChannelSnCode;

    std::string eightChannelFrameMes;

    //global
    QFont *pushbutton_font;

    QFont *showlabel_font;
};

void showMessage(QString str,bool flag);
#endif // SETTINGS_H

