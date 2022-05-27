#ifndef AUTOPACK_STEP_ONE_H
#define AUTOPACK_STEP_ONE_H

#include <QWidget>
#include <wzserialport.h>
namespace Ui {
class autopack_step_one;
}

class autopack_step_one : public QWidget
{
    Q_OBJECT

public:
    explicit autopack_step_one(QWidget *parent = nullptr);
    ~autopack_step_one();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_returnPressed();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void load_mes();
private:
    Ui::autopack_step_one *ui;
    long long pre_Time;
    QVector<QChar> sl;
    long rowcount;
    bool rowComplete;
    MultipleProcessStruct multipleprocessstruct;
};

#endif // AUTOPACK_STEP_ONE_H
