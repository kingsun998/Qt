#ifndef STANDARSET_H
#define STANDARSET_H

#include <QWidget>


namespace Ui {
class standarset_ui;
}

class standarSet:public QWidget
{
public:
    standarSet(QWidget *parent=nullptr);

private slots:
    void click(bool check);
public slots:
    void LoadEightPortMes();
private:
    Ui::standarset_ui *ui;
};

#endif // STANDARSET_H
