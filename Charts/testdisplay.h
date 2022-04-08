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
#include <QComboBox>
#include <datastruct.h>
#include <QTimer>

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
signals:
    void sendMessage(ulong,CAN_OBJ,QString);
    void SendToFrame(ulong,CAN_OBJ,QString,QString);
    void sendToTable(int lineId,int canid,double deviceid,double date);
    void sendtochart(int mx,uint index,double val);
private slots:
    void handleTimeOut();
    void handleSenderTimeOut();
//  控制测试的开始和暂停
    void shspline(int,bool);
    void show_detail(uint mx,CAN_OBJ obj,QString datetime);
    //Can操作
    void connectCan();
    void disconnectCan();
    void startCan();
    void YscaleJump();

//    表示按下了开始测试按钮
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_radioButton_clicked(bool checked);
private:
    void sendSingleFrameToCan();
private:
    Ui::TestDisplay *ui;
    QChartView * chartview;
    Mychart * chart;

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
    bool IfRun;
    //需要用到的名称
    QVector<QString> company_usedate;
    QVector<QString> company_date;
    //接受数据的容器
    QVector<QVector<QLabel *>>  showLabel;
    QVector<QString> ReceiveTimestamp;
    //信息传递的结构
    DataStruct  ReceiveVal;
    //下标
    ulong m_x;
    //接受的总帧数
    int totalnums;

    //savetime;
    //接受的数据
    double saveTime;
    double saveInterval_miseconds;  //以毫秒计算的间隔
    QString companyname;
    bool PrecesionMode=false;
    double TestTemperature=0;
    double offset=0;

    QMap<QString,QMap<QString,byte>> frameConfig;
    //发送计时器
    QTimer *sendtimer;
    uint sendtimes;
};

#endif // TESTDISPLAY_H
