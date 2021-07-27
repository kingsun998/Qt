#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <baseinfo.h>
#include <settings.h>
#include <chartdisplay.h>
#include <standarset.h>
#include <framedispaly.h>
#include <baseinfo.h>
#include <sendmessages.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
