#ifndef MODE_SETTINGS_H
#define MODE_SETTINGS_H

#include <QWidget>
#include <utils.h>
namespace Ui {
class mode_settings;
}

class mode_settings : public mode_base
{
    Q_OBJECT

public:
    explicit mode_settings(QWidget *parent = nullptr);
    ~mode_settings() override;
    virtual void reset() override;
    virtual void init() override;
private:
    Ui::mode_settings *ui;
};

#endif // MODE_SETTINGS_H
