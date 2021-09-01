#ifndef FRAMEDISPALY_H
#define FRAMEDISPALY_H

#include <QWidget>
#include <qstandarditemmodel.h>
#include <settings.h>
#include <qtimer.h>
#include <comprocess.h>
#include <QMap>
#include <QTableWidgetItem>
namespace Ui{
    class framedisplay_ui;
}

class frameDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit frameDisplay(QWidget *parent = nullptr);

signals:
    void sendMesToCom(QString mes);

public slots:
    void getMessage(int mx,CAN_OBJ obj,QString datetime,int companycode);

private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_comboBox_3_activated(int index);

    void on_comboBox_4_activated(int index);

    void on_spinBox_4_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);
    //发送数据
    void on_pushButton_5_clicked();
    //清楚计数
    void on_pushButton_3_clicked();
    //停止发送
    void on_pushButton_6_clicked();

    void receiveMesFromCom(QVariant var);

    void dispaly_comResult(QString str);
private:
//    QStandardItemModel* model;
    Ui::framedisplay_ui *ui;
    int rowcount;
    bool dispaly_allowshow;
    QVector<QString> FrameType;
    QVector<QString> CompanyName;
    QVector<uint> FrameID;
    QVector<uint> FrameLen;
    QVector<QString> FrameContent;
    QVector<QString> Data;
    bool dispaly_ifreceive;

    //计时器
    QTimer *timer;
    DWORD timeClick;
    double saveInterval_miseconds;


    //发送信息属性
    QString send_frame_type;
    QString send_frame_format;
    uint send_frame_id;
    uint send_frame_interval;
    uint send_frame_nums;
    int send_nums;
    int receive_nums;
    bool ifsend;

    //串口
    ComProcess *comprocess;
    ComParameter parameter;
    bool switchReceive;


    //记录item
    QList<QVector<QTableWidgetItem *>> list;
};

#endif // FRAMEDISPALY_H
