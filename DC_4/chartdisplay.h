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
#include <datastruct.h>
#include <qglobal.h>
#include <QGroupBox>
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
    int Calculate(int index,double y);
signals:
    void sendMessage(long,CAN_OBJ,QString,int);
    void sendToTable(int lineId,int canid,double deviceid,double date);
    void sendtochart_first(int mx,int index,double val,int type);
    void sendtochart_second(int mx,int index,double val,int type);
    void sendsingleframetocan(long index,CAN_OBJ obj,QString datetime,int companycode);
private slots:
    void handleTimeOut();
    //控制测试的开始和暂停
    void on_pushButton_2_clicked(bool checked);
    void shspline(int,bool);
    void show_detail(long mx,CAN_OBJ obj,QString datetime,int companycode);
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void changeCompanyType();

    int PrecesionTest(int index,double val);

    void on_comboBox_currentIndexChanged(int index);

    void on_radioButton_toggled(bool checked);

    void on_lineEdit_3_editingFinished();

    void getPrePrecesionTime();

    void handlePrePrecesionTimeOut();

    void on_pushButton_6_clicked();

    void sendSingleFrameToCan();

    void handleSenderTimeOut();

    void on_radioButton_3_clicked(bool checked);

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
    QVBoxLayout vboxlayout[6];
    QGroupBox groupbox[6];

    //测试开关
    bool RunTest;
    QVector<QLabel *> label_Val;
    QVector<QLabel *> label_Status;
    QVector<QLabel *> label_Time;
    QVector<QLabel *> label_testStatus;
    QVector<QLabel *> label_testError;
    //下标
    uint m_x;
    //接受的总帧数
    int totalnums;

    //接受的数据
    QVector<double> timestart;
    QVector<bool> records_bt;
    QVector<double> timeend;
    QVector<bool> records_tp;
    QVector<QString> receivetime;
    //savetime;
    //接受的数据
    double saveTime;
    DataStruct ReceiveVal_A;
    DataStruct ReceiveVal_B;
    double saveInterval_miseconds;  //以毫秒计算的间隔

    CAN_OBJ objs[50];
    int companytypecode;

    void resetAry();

    bool PrecesionMode;
    double TestTemperature;
    double offset;

    int current_compnay;

    //test
    QVector<bool> precisionStatus;
    QVector<double> precisionError;
    QTimer pretimer;
    bool IfTestPrePrecesion;
    int pretime;

    int sendtimes;
    QTimer *sendtimer;

    //
    QMap<QString,QMap<QString,byte>> frameConfig;
};

#endif // CHARTDISPLAY_H
