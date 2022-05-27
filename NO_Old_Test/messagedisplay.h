#ifndef MESSAGEDISPLAY_H
#define MESSAGEDISPLAY_H

#include <QWidget>

namespace Ui {
class MessageDisplay;
}

class MessageDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit MessageDisplay(QWidget *parent = nullptr);
    ~MessageDisplay();

private:
    Ui::MessageDisplay *ui;
    long index;
};

#endif // MESSAGEDISPLAY_H
