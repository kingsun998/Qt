#ifndef BASEINFO_H
#define BASEINFO_H

#include <QWidget>
#include <settings.h>
#include <QPushButton>
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

    void on_lineEdit_5_editingFinished();

    void on_comboBox_8_activated(int index);

    void on_lineEdit_6_editingFinished();

    void on_radioButton_clicked(bool checked);


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_10_editingFinished();

    void on_lineEdit_11_editingFinished();

    void on_lineEdit_12_editingFinished();

    void on_lineEdit_13_editingFinished();

private:
    Ui::baseinfo_ui *ui;

    QVector<QPushButton*> pushbutton_list;
};

#endif // BASEINFO_H
