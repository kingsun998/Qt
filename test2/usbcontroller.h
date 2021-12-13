#ifndef USBCONTROLLER_H
#define USBCONTROLLER_H

#include <QObject>

class UsbController : public QObject
{
    Q_OBJECT
public:
    explicit UsbController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // USBCONTROLLER_H
