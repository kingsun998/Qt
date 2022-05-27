#ifndef AUTOPACKING_STEP_THREE_H
#define AUTOPACKING_STEP_THREE_H

#include <QWidget>

namespace Ui {
class autopack_step_three;
}

class autopack_step_three : public QWidget
{
    Q_OBJECT

public:
    explicit autopack_step_three(QWidget *parent = nullptr);
    ~autopack_step_three();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_returnPressed();

private:
    Ui::autopack_step_three *ui;
    long long pre_Time;
    QVector<QChar> sl;
};

#endif // AUTOPACKING_STEP_THREE_H
