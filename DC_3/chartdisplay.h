#ifndef CHARTDISPLAY_H
#define CHARTDISPLAY_H

#include <qwidget.h>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <mychart.h>
#include <QTimer>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QCheckBox>
#include <qlabel.h>
#include <qlist.h>
#include <qglobal.h>
#include <QGroupBox>
#include <datastruct.h>
#include <dbcontroller.h>
namespace Ui {
class chartdisplay_ui;
}

QT_CHARTS_USE_NAMESPACE

class chartDisplay : public QWidget
{
    Q_OBJECT
public:
    chartDisplay(QWidget *parent= nullptr);
    ~chartDisplay();
    void Calculate(int index,double y);
signals:
    void sendMessage(ulong,CAN_OBJ,QString,int);
    void sendToTable(int lineId,int canid,double deviceid,double date);
    void sendtochart(int chartype,int mx,int index1,double tcf,double tcs,double tct);
//    表示按下了开始测试按钮
    void setFrameStartTime();
private slots:
    void handleTimeOut();
    //控制测试的开始和暂停
    void on_pushButton_2_clicked(bool checked);
    void shspline(int,bool);
    void show_detail(uint mx,CAN_OBJ obj,QString datetime,int companycode);
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

//    void changeCompanyType();
    void suit_Cell(int chartype,int code,uint mx,int index1,int low1,int high1,
                   int index2,int low2,int high2,BYTE *date);

    void on_comboBox_currentIndexChanged(int index);

    void on_radioButton_toggled(bool checked);

    void on_lineEdit_3_editingFinished();

private:
    Ui::chartdisplay_ui *ui;

    Mychart *fchart;
    Mychart *schart;
    QChartView *fchartView;
    QChartView *schartView;

    //
    QButtonGroup *btg;

    QLabel *label;
    QTimer *timer;
    QVector<QCheckBox *> pb;
    QVBoxLayout vboxlayout[4];
    QGroupBox groupbox[4];

    //测试开关
    bool RunTest;

    QVector<QLabel *> ReceiveVal;
    QVector<QLabel *> ReceiveStatus;
    QVector<QLabel *> ReceiveTime;
    //缓存
    QVector<int>  show_clock;
    //下标
    uint m_x;
    //接受的总帧数
    int totalnums;

    //接受的数据
    QVector<double> timestart;
    QVector<bool> records_bt;
    QVector<double> timeend;
    QVector<bool> records_tp;

    //savetime;
    //接受的数据
    double saveTime;
    QVector<double> lastrecord;

    DataStruct  Receive;
    QVector<QString> ReceiveTimestamp;
    double saveInterval_miseconds;  //以毫秒计算的间隔

    int companytypecode;

    void resetAry();

    bool PrecesionMode;
    double TestTemperature;
    double offset;


};

#endif // CHARTDISPLAY_H
