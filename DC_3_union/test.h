#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include <scangun.h>
#include <wzserialport.h>
#include <QEventLoop>
#include <mqttcommunication.h>
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
    //发送数据
    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

//    void on_pushButton_4_clicked();

    void on_pushButton_8_clicked();

    void on_comboBox_3_currentIndexChanged(int index);

    void on_pushButton_4_clicked();

public slots:
    void LoadSinglePortMes();
    void LoadEightPortMes();
    void scangunClose();
//    void scangun2Close();
private:
    Ui::Test *ui;
    ScanGun *scangun;
//    ScanGun *scangun2;
    QVector<QString> waterinsulation;
    QVector<QString> comport_names;
    bool openport;

    QEventLoop el;
    long rowcount_eight;
    long rowcount_single;
    int water_index;

    EightPortStruct eightportstruct;
    SinglePortStruct singleportstruct;

    //water
    QVector<QString> preSN;
    //
    QVector<QPushButton*> pushbutton_list;
};

#endif // TEST_H
