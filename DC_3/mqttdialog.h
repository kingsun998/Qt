#ifndef MQTTDIALOG_H
#define MQTTDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QtMqtt/qmqttclient.h>
namespace Ui {
class mqttDialog;
}

class mqttDialog : public QDialog
{
    Q_OBJECT

public:
    explicit mqttDialog(QWidget *parent = nullptr);
    ~mqttDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::mqttDialog *ui;
    QMqttClient *m_client;
};

#endif // MQTTDIALOG_H
