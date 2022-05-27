#ifndef MODE_SENDING_H
#define MODE_SENDING_H

#include <QWidget>
#include <utils.h>
namespace Ui {
class mode_sending;
}

class mode_sending : public mode_base
{
    Q_OBJECT

public:
    explicit mode_sending(QWidget *parent = nullptr);
    ~mode_sending() override;
    virtual void reset() override;
    virtual void init() override;
private:
    Ui::mode_sending *ui;
};

#endif // MODE_SENDING_H
