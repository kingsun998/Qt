#ifndef FRAMEDISPALY_H
#define FRAMEDISPALY_H

#include <QWidget>
#include <qstandarditemmodel.h>
#include <settings.h>
#include <qtimer.h>
#include <comprocess.h>
#include <QMap>
#include <QTableWidgetItem>
#include <subcomdialog.h>
#include <dbcontroller.h>

typedef  struct {
    bool init=false;
    bool running=false;
    bool stop=true;
}Status;


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
    void writeToTable(ulong index,CAN_OBJ obj,QString datetime,int companycode);
public slots:
    void getMessage(ulong index,CAN_OBJ obj,QString datetime,QString companyname);
    void sendHandleTimeOut();
    void sendCan();
    void sendMessage(QVariant var);
    void setStartTime();
private slots:
    // 是否向表格添加内容
    void on_pushButton_2_clicked();
    // 保存表格按钮
    void on_pushButton_clicked();

    //清楚计数
    void on_pushButton_3_clicked();
    //发送数据
    void on_pushButton_6_clicked();
    //是否创建设置参数窗口
    void on_pushButton_4_clicked();
    void receiveMesFromCom(QVariant var);
    void on_pushButton_5_clicked();

    void dispaly_comResult(QVariant var,int arraytype);
    //波特率
    void on_comboBox_currentIndexChanged(const QString &arg1);
    //选择串口
    void on_comboBox_3_currentIndexChanged(const QString &arg1);
    //校验位
    void on_comboBox_5_currentIndexChanged(const QString &arg1);
    //数据位
    void on_comboBox_2_currentIndexChanged(const QString &arg1);
    //停止位
    void on_comboBox_4_currentIndexChanged(const QString &arg1);
    //初始化CAN
    void on_pushButton_8_clicked();
    //关闭CAN
    void on_pushButton_9_clicked();
    //CAN发送数据
    void on_pushButton_7_clicked();
    //CAN停止发送
    void on_pushButton_10_clicked();
    //改变发送ID
    void on_spinBox_valueChanged(int arg1);

public:
    uint QstringToInt(QString str);


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
    QVector<bool> ifFrameReceive;
    bool dispaly_ifreceive;

    std::mutex mtx;
    //计时器
    QTimer *sendtimer;
    DWORD startTime;
    double saveInterval_miseconds;
    Status status;


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
    ComParameter parameter;
    bool switchReceive;
    //CAN send
    CAN_OBJ objs[50];
    //记录item
//    QList<QVector<QTableWidgetItem *>> list;

    //子窗口
    SubComDialog *dialog;
    ULONG index;
};
#endif // FRAMEDISPALY_H
