#ifndef AUTOPACKING_H
#define AUTOPACKING_H

#include <QWidget>
#include "autopack_step_one.h"
#include "autopack_step_two.h"
#include "autopack_step_three.h"

namespace Ui {
class autoPacking;
}

class autoPacking : public QWidget
{
    Q_OBJECT

public:
    explicit autoPacking(QWidget *parent = nullptr);
    ~autoPacking();

private:
    Ui::autoPacking *ui;
    autopack_step_one *step_one;
    autopack_step_two *step_two;
    autopack_step_three *step_three;
};

#endif // AUTOPACKING_H
