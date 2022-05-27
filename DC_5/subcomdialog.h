#ifndef SUBCOMDIALOG_H
#define SUBCOMDIALOG_H

#include <QDialog>
#include <QVariant>

typedef struct{
    int heatPlc=-1;
    int coldPlc=-1;
    long totalTestPlc=-1;
    int changeChannelPlc=-1;
    int hourPlc=-1;
    int minutePlc=-1;
    int secondPlc=-1;
    int upHeatPipeMoveDown=-1;
    int leftHeatPipeMoveForward=-1;
}Message;

namespace Ui {
class SubComDialog;
}

class SubComDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SubComDialog(QWidget *parent = nullptr);
    ~SubComDialog();
signals:
    void SendComMes(QVariant var);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SubComDialog *ui;
};

#endif // SUBCOMDIALOG_H
