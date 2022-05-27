#ifndef DEVICEASSEMBLY_H
#define DEVICEASSEMBLY_H

#include <QWidget>
#include <QCheckBox>
#include <QVector>
#include <QLabel>
#include <QButtonGroup>
#include <qtimer.h>
#include "mychart.h"
#include <ECanVci.h>
namespace Ui {
class deviceAssembly;
}

class deviceAssembly : public QWidget
{
    Q_OBJECT
public:
    explicit deviceAssembly(QWidget *parent = nullptr);
    ~deviceAssembly();
    //Can操作
    void connectCan();
    void disconnectCan();
    void startCan();
    void YscaleJump();
    void change_Company(int);
signals:
    void sendtochart(ulong mx,uint index,double val);
    void sendMessage(ulong,CAN_OBJ,QString);
public slots:
    void shspline(int,bool);
    void handleTimeOut();
    void show_detail(ulong mx,CAN_OBJ obj,QString datetime);
    void precisionTestSwitch(bool state);
    double lowPassFilter(int index,double val);
    int PrecesionTest(int index,double val);
    void setSensorState(int index,std::map<int,QString>& map,int val);
    void setnotest(int index);
    void handlePreTimeOut();
    void processTest(int index1,double Tcf,int index2,double Tcs,ulong mx);
private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_4_editingFinished();

    void on_lineEdit_5_editingFinished();

    void on_lineEdit_textChanged(const QString &arg1);


    void on_lineEdit_returnPressed();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::deviceAssembly *ui;
    QVector<QCheckBox*> checkboxList;
    QVector<QLabel*> testValue;
    QVector<QLabel*> diffValue;
    QVector<QLabel*> testState;
    QVector<QLabel*> sensorState;

    QButtonGroup *buttongroup;
    Mychart *mychart;

    bool IfRun;
    QTimer timer;
    QTimer pretimer;
    DWORD saveTime;

    int company_type;
    QString company_name;

    QVector<bool> precisionStatus;

    bool IfTestPrecision;
    bool IfTestPrePrecesion;
    int TestTemperature;
    int pretime;
    double offset;
    int saveInterval_miseconds;
    ulong m_x;

    QVector<double> recordVal;
    QVector<int> show_clock;
    QVector<double> recordValTemp;
    QVector<double> precisionError;
    QVector<double> precisionDiff;

    long long pre_Time;
    QVector<QChar> sl;
    long rowcount;
    bool rowComplete;

    QVector<QLabel*> idlabel;
};

#endif // DEVICEASSEMBLY_H
