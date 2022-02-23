#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qvector.h>
#include <QTime>
#include <qwidget.h>
#include <QLabel>
#include <mutex>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_lineEdit_4_returnPressed();

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_groupBox_clicked();


private:

    QVector<QLabel *> input_list;
private:
    Ui::MainWindow *ui;


    int at;

    void next(QString s);

    long long  pre_Time;
    QString pre_mes;
    long long  temp;
    QVector<QChar> sl;
    std::mutex lock;
};

#endif // MAINWINDOW_H
