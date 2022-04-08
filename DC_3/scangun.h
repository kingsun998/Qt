#ifndef DEVICEUNION_H
#define DEVICEUNION_H

#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QSet>

namespace Ui {
class ScanGun;
}

class ScanGun : public QDialog
{
    Q_OBJECT

public:
    explicit ScanGun(QWidget *parent=nullptr,QVector<QString> *mes=nullptr);
    ~ScanGun();
    void getSensorMes();
private slots:
    void on_lineEdit_step1_returnPressed();

    void on_lineEdit_step1_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();


private:
    QVector<QLabel *> input_list;
    QVector<QLineEdit *> lineedit_list;
private:
    QVector<QString> *Mes;
    Ui::ScanGun *ui;
    int at;
    void next(QString s);
    long long  pre_Time;
    QString pre_mes;
    long long  temp;
    QVector<QChar> sl;
    QWidget *parent;
    QSet<QString> sensorId;
};

#endif // DEVICEUNION_H
