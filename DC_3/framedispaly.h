#ifndef FRAMEDISPALY_H
#define FRAMEDISPALY_H

#include <QWidget>
#include <qstandarditemmodel.h>
#include <settings.h>
#include <qtimer.h>
namespace Ui{
    class framedisplay_ui;
}

class frameDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit frameDisplay(QWidget *parent = nullptr);

signals:

public slots:
    void getMessage(int mx,CAN_OBJ obj,QString datetime,int companycode);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
//    QStandardItemModel* model;
    Ui::framedisplay_ui *ui;
    int rowcount;
    bool allowshow;
    QVector<QString> FrameType;
    QVector<QString> CompanyName;
    QVector<uint> FrameID;
    QVector<uint> FrameLen;
    QVector<QString> FrameContent;
    QVector<QString> Data;
    bool receive;

    //计时器
    QTimer *timer;
    DWORD timeClick;
    double saveInterval_miseconds;
};

#endif // FRAMEDISPALY_H
