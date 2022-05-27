#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <baseinfo.h>
#include <settings.h>
#include <chartdisplay.h>
#include <standarset.h>
#include <framedispaly.h>
#include <baseinfo.h>
#include <bootloader.h>
#include <deviceunion.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void mergeFileRequest();


private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *mergeFiletimer;
};

#endif // MAINWINDOW_H
