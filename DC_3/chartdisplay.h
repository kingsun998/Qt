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
#include <dbservice.h>
#include <qglobal.h>

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
    void sendMessage(uint,CAN_OBJ,QString);
    void sendToTable(int lineId,int canid,double deviceid,double date);
    void sendtochart(int chartype,int mx,int index1,double tcf,double tcs);
private slots:
    void handleTimeOut();
    //控制测试的开始和暂停
    void on_pushButton_2_clicked(bool checked);
    void shspline(int,bool);
    void show_detail(uint mx,CAN_OBJ obj,QString datetime);
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::chartdisplay_ui *ui;

    Mychart *fchart;
    Mychart *schart;
    QChartView *fchartView;
    QChartView *schartView;

    //
    QButtonGroup *btg;
    QCheckBox *pb[7];

    QLabel *label;

    QTimer *timer;
    //下标
    uint m_x;

    //接受的总帧数
    int totalnums;

    //测试开关
    bool iftest;

    double timestart[7];
    bool records_bt[7];
    double timeend[7];
    bool records_tp[7];

    //savetime;
    double saveTime;
    QVector<QVector<double>> saveTempeture;
    QVector<QVector<QString>> saveTimestamp;
    QVector<QVector<QString>> status;
    QVector<QVector<double>> time;
    double saveInterval_miseconds;  //以毫秒计算的间隔

    CAN_OBJ objs[50];
};

#endif // CHARTDISPLAY_H
