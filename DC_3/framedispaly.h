#ifndef FRAMEDISPALY_H
#define FRAMEDISPALY_H

#include <QWidget>
#include <qstandarditemmodel.h>
#include <settings.h>
namespace Ui{
    class framedisplay_ui;
}

class frameDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit frameDisplay(QWidget *parent = nullptr);

signals:

public slots:
    void getMessage(int mx,CAN_OBJ obj,QString datetime);
private:
//    QStandardItemModel* model;
    Ui::framedisplay_ui *ui;
    int rowcount;
};

#endif // FRAMEDISPALY_H
