#ifndef BASEINFO_H
#define BASEINFO_H

#include <QWidget>
#include <settings.h>
namespace Ui {
class baseinfo_ui;
}

class baseInfo : public QWidget
{
    Q_OBJECT
public:
    explicit baseInfo(QWidget *parent = nullptr);

signals:

public slots:


private slots:
    void on_lineEdit_2_editingFinished();

    void on_lineEdit_3_editingFinished();

    void on_lineEdit_4_editingFinished();

private:
    Ui::baseinfo_ui *ui;
};

#endif // BASEINFO_H
