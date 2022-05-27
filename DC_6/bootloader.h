#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <QtGui>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <include/usb2xxx/can_bootloader.h>
#include <include/usb2xxx/usb_device.h>
#include <include/usb2xxx/scandevrangedialog.h>
#include <include/usb2xxx/dialogaddnode.h>

namespace Ui {
class bootloader_ui;
}

class BootLoader : public QWidget
{
    Q_OBJECT
    
public:
    explicit BootLoader(QWidget *parent = 0);
    ~BootLoader();
    void ScanDevice(void);
    bool DeviceConfig(void);
    void resizeEvent(QResizeEvent *pEvent);
    QString GetErrorString(int ErrorCode);
private slots:
    void on_openFirmwareFilePushButton_clicked();

    void on_updateFirmwarePushButton_clicked();

    void on_openFirmwareFileAction_triggered();

    void on_scanNodeAction_triggered();

    void on_contactUsAction_triggered();

    void on_aboutAction_triggered();

    void on_exitAction_triggered();

    void on_actionAddNode_triggered();

    void on_comboBoxIDType_currentIndexChanged(int index);

    void on_actionScanDevice_triggered();

    void on_actionDocument_triggered();

    void on_biaodingpushButton_clicked();


    void on_pushButton_clicked();


private:
    Ui::bootloader_ui *ui;
};

#endif // MAINWINDOW_H
