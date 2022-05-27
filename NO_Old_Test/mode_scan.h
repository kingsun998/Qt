#ifndef MODE_SCAN_H
#define MODE_SCAN_H

#include <QWidget>
#include <utils.h>
#include <qlabel.h>
#include <QLineEdit>
namespace Ui {
class mode_scan;
}

class mode_scan : public mode_base
{
    Q_OBJECT

public:
    explicit mode_scan(QWidget *parent = nullptr);
    ~mode_scan() override;
   virtual void reset() override;
   virtual void init() override;
   QVector<QString>& getSensorMes();
private:
    Ui::mode_scan *ui;

signals:
    void closeTheDialog();
private slots:
    void on_lineEdit_step1_returnPressed();

    void on_lineEdit_step1_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    QVector<QLabel *> input_list;
    QVector<QLineEdit *> lineedit_list;

    QVector<QString> Mes;
    int at;
    void next(QString s);
    long long  pre_Time;
    QString pre_mes;
    long long  temp;
    QVector<QChar> sl;
    QWidget *parent;
    QSet<QString> sensorId;

};

#endif // MODE_SCAN_H
