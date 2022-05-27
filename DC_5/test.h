#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include <scangun.h>
#include <wzserialport.h>
#include <QEventLoop>
#include <mqttcommunication.h>
#include <qtimer.h>
namespace Ui {
class Test;
}

class Test : public QWidget
{
    Q_OBJECT

public:
    explicit Test(QWidget *parent = nullptr);
    ~Test();

private slots:
    void on_pushButton_clicked();
    //加载数据
    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();


    void on_comboBox_3_currentIndexChanged(int index);

    void on_pushButton_4_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

public slots:
    void LoadSinglePortMes();
    void LoadEightPortMes();
    void scangunClose();
    void scangun2Close();
    void handleTimeOut();
private:
    Ui::Test *ui;
    ScanGun *scangun;
    ScanGun *scangun2;

    QVector<QString> highTempInsulation;
    QVector<QString> waterinsulation;

    QVector<SinglePortStruct> waterinsulationUnIon;
    QVector<SinglePortStruct> highTempInsulationUnIon;
    QVector<QString> waterSensorIdUnion;
    QVector<QString> highSensorIdUnion;

    QEventLoop el;
    long rowcount_eight;
    long rowcount_single;
    int water_index;
    int highTemp_index;

    SinglePortStruct singleportstruct_water;
    SinglePortStruct singleportstruct_hightemp;

    //更改button的样式
    QVector<QPushButton*> pushbutton_list;

    QTimer timer;
};

#endif // TEST_H
