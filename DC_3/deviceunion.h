#ifndef DEVICEUNION_H
#define DEVICEUNION_H

#include <QWidget>
#include <qvector.h>
#include <QTime>
#include <qwidget.h>
#include <QLabel>
#include <mutex>

namespace Ui {
class DeviceUnion;
}

class DeviceUnion : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceUnion(QWidget *parent = nullptr);
    ~DeviceUnion();
private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_lineEdit_4_returnPressed();

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_groupBox_clicked();

private:

    QVector<QLabel *> input_list;
private:
    Ui::DeviceUnion *ui;


    int at;

    void next(QString s);

    long long  pre_Time;
    QString pre_mes;
    long long  temp;
    QVector<QChar> sl;
    std::mutex lock;
};

#endif // DEVICEUNION_H
