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

public slots:
    void getMessage(int mx,CAN_OBJ obj,QString datetime,int companycode);
    void sendMessage(QVariant var);
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

    //记录item
    QList<QVector<QTableWidgetItem *>> list;

    //子窗口
    SubComDialog *dialog;
};
#endif // FRAMEDISPALY_H
