#ifndef FRAMEDISPALY_H
#define FRAMEDISPALY_H

#include <QWidget>
#include <qstandarditemmodel.h>
#include <settings.h>
#include <qtimer.h>
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
    void setStartTime();
private slots:
    // 是否向表格添加内容
    void on_pushButton_2_clicked();
    // 保存表格按钮
    void on_pushButton_clicked();

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
//    QTimer *sendtimer;
    DWORD startTime;
    double saveInterval_miseconds;


};
#endif // FRAMEDISPALY_H
