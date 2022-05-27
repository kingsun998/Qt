#ifndef MODE_UPLOAD_H
#define MODE_UPLOAD_H

#include <QWidget>
#include <utils.h>
#include <QtMqtt/qmqttclient.h>


namespace Ui {
class mode_upload;
}

class mode_upload : public mode_base
{
    Q_OBJECT

public:
    explicit mode_upload(QWidget *parent = nullptr);
    ~mode_upload() override;
    virtual void reset() override;
    virtual void init() override;

    void publish_Mes(MqttDataStruct& data);
    void reset_config(QString ip_address,QString port,QString topic,int qos);
    void connectToHost();
    void scribe();
public slots:
    void updateLogStateChange();
private slots:
    void on_pushButton_clicked();

private:
    Ui::mode_upload *ui;

    QMqttClient *m_client;
    QMqttSubscription* subscription;

    QString ip_address;
    quint16 port;
    QString topic;
    int qos;

};

#endif // MODE_UPLOAD_H
