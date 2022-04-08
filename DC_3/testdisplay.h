#ifndef TESTDISPLAY_H
#define TESTDISPLAY_H

#include <QWidget>
#include <mychart.h>
#include <QChartView>
#include <QButtonGroup>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <datastruct.h>
#include <QComboBox>
#include <scangun.h>
#include <mqttcommunication.h>

namespace Ui {
class TestDisplay;
}

class TestDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit TestDisplay(QWidget *parent = nullptr);
    ~TestDisplay();
    double Calculate(int index,double y);
    QVector<QString>* getScanMes();
signals:
    void sendMessage(ulong,CAN_OBJ,QString);
    void SendToFrame(ulong,CAN_OBJ,QString,QString);
    void sendToTable(int lineId,int canid,double deviceid,double date);
    void sendtochart_First_Up(int mx,uint index,double val);
    void sendtochart_First_Down(int mx,uint index,double val);
    void sendtochart_Second_Up(int mx,uint index,double val);
    void sendtochart_Second_Down(int mx,uint index,double val);
private slots:
    void handleTimeOut();
//  控制测试的开始和暂停
    void shspline(int,bool);
    void show_detail(uint mx,CAN_OBJ obj,QString datetime);
    void change_Company(int company_id);
    void highTempTestSwitch(bool state);
    void precisionTestSwitch(bool state);
    void newscan();
    void senddata();
    //Can操作
    void connectCan();
    void disconnectCan();
    void startCan();
    void YscaleJump();
    //测试内容
    int PrecesionTest(int index,double val);
    int highTempStableTest(int index,double val);
    void sensorState(int index,std::map<int,QString>& map,int val);
    void setnotest(int index);
    void getPrecesion();
    //pretime
    void getHighThreshold();
    void getPrePrecesionTime();
    void handlePrePrecesionTimeOut();
    //低通滤波
    double lowPassFilter(int index,double val);
    void resetScangunmes();
//    表示按下了开始测试按钮
private:
    Ui::TestDisplay *ui;
    QVector<QChartView *> chartviews;
    QVector<QWidget *> bodyblocks;

    QButtonGroup *buttongroup;

    QLabel *label;
    QTimer *timer;
    //表格数组
    QVector<Mychart *> mycharts;
    QVector<QCheckBox *> checkboxs;
    QVector<QVBoxLayout *> vboxlayout;
    QVector<QGridLayout *> gridlayout;
    QVector<QGridLayout *> gridlayout2;
    QVector<QGroupBox *> groupboxs;
    QVector<QGroupBox *> groupboxs2;
    QVector<QHBoxLayout *> hboxlayout;
    QVector<QLineEdit *> lineedit;
    QVector<QPushButton *> pushbuttons;
    QMap<QString,int> buttonmap;
    QMap<QString,int> lineeditmap;
    //测试开关
    bool IfRun;
    //需要用到的名称
    QVector<QString> company_usedate;
    QVector<QString> company_date;
    //接受数据的容器
    QVector<QVector<QLabel *>>  showLabel;
    QVector<QVector<QString>> ReceiveTimestamp;
    //信息传递的结构
    DataStruct  ReceiveVal_A1;
    DataStruct  ReceiveVal_A2;
    DataStruct  ReceiveVal_B1;
    DataStruct  ReceiveVal_B2;
    //下标
    ulong m_x;
    //接受的总帧数
    int totalnums;

    //时间记录
    QVector<double> timestart;
    QVector<bool> records_bt;
    QVector<double> timeend;
    QVector<bool> records_tp;

    //savetime;
    //接受的数据
    double saveTime;
    double saveInterval_miseconds;  //以毫秒计算的间隔
    QString companyname;
    bool PrecesionMode=false;
    double TestTemperature=0;
    double offset=0;

    //切换公司
    QVector<QComboBox *> combobox;

    //低通滤波
    QVector<double> recordVal;
    QVector<double> recordValTemp;
    //测试模式
    int testMode;
    //高温稳定性测试
    QCheckBox *highTempCheckBox;
    bool IfTestHighTemp;
    QVector<double> highTempTest;
    QVector<bool> highTempStatus;

    //精度测试
    QCheckBox *precisionCheckBox;
    QVector<bool> precisionStatus;
    QVector<double> precisionError;
    bool IfTestPrecision;
    //show_clock
    QVector<int> show_clock;
    //预精度测试
    bool IfTestPrePrecesion;
    int pretime;
    QTimer pretimer;
    QVector<QString> testname;
    QVector<QCheckBox*> testcheckbox;
    //扫描枪
    ScanGun *scangun;
    QVector<QString> scangunmes;
    //mqtt
    //MqttCommunication *mqtt;

    int company_type;
};

#endif // TESTDISPLAY_H
