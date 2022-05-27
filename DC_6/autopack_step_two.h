#ifndef AUTOPACK_STEP_TWO_H
#define AUTOPACK_STEP_TWO_H

#include <QWidget>

namespace Ui {
class autopack_step_two;
}

class autopack_step_two : public QWidget
{
    Q_OBJECT

public:
    explicit autopack_step_two(QWidget *parent = nullptr);
    ~autopack_step_two();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_returnPressed();

private:
    Ui::autopack_step_two *ui;
    long long pre_Time;
    QVector<QChar> sl;
};

#endif // AUTOPACK_STEP_TWO_H
