#include "mqttdialog.h"
#include "ui_mqttdialog.h"
#include "qdatetime.h"
#include "QMessageBox"
mqttDialog::mqttDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mqttDialog)
{
    ui->setupUi(this);
    m_client = new QMqttClient(this);
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
           const QString content = QDateTime::currentDateTime().toString()
                       + QLatin1String(" Received Topic: ")
                       + topic.name()
                       + QLatin1String(" Message: ")
                       + message
                       + QLatin1Char('\n');
           ui->textEdit->insertPlainText(content);
       });
}

mqttDialog::~mqttDialog()
{
    delete ui;
}

void mqttDialog::on_buttonBox_clicked(QAbstractButton *button)
{

    m_client->setHostname(ui->lineEdit->text());
    m_client->setPort(ui->lineEdit_2->text().toUInt());

    auto subscription = m_client->subscribe(ui->lineEdit_3->text());
    if (!subscription) {
        QMessageBox::critical(this, QLatin1String("Error"),
                              QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

}
