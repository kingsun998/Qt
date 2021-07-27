#-------------------------------------------------
#
# Project created by QtCreator 2021-07-21T16:38:50
#
#-------------------------------------------------

QT       += core gui
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DC_3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        baseinfo.cpp \
        chartdisplay.cpp \
        dbservice.cpp \
        framedisplay.cpp \
        main.cpp \
        mainwindow.cpp \
        mychart.cpp \
        sendmessages.cpp \
        settings.cpp \
        standarset.cpp

HEADERS += \
        ECanVci.h \
        baseinfo.h \
        chartdisplay.h \
        dbservice.h \
        framedispaly.h \
        include/libxl/IAutoFilterT.h \
        include/libxl/IBookT.h \
        include/libxl/IFilterColumnT.h \
        include/libxl/IFontT.h \
        include/libxl/IFormatT.h \
        include/libxl/IRichStringT.h \
        include/libxl/ISheetT.h \
        include/libxl/enum.h \
        include/libxl/libxl.h \
        include/libxl/setup.h \
        include/usb2xxx/ControlCAN.h \
        include/usb2xxx/LDFDecoder.h \
        include/usb2xxx/can_bootloader.h \
        include/usb2xxx/can_uds.h \
        include/usb2xxx/i2c_sniffer.h \
        include/usb2xxx/ir_sniffer.h \
        include/usb2xxx/lin_uds.h \
        include/usb2xxx/offline_type.h \
        include/usb2xxx/uart_sniffer.h \
        include/usb2xxx/usb2adc.h \
        include/usb2xxx/usb2ads1256.h \
        include/usb2xxx/usb2can.h \
        include/usb2xxx/usb2canfd.h \
        include/usb2xxx/usb2cnt.h \
        include/usb2xxx/usb2dac.h \
        include/usb2xxx/usb2ew.h \
        include/usb2xxx/usb2gpio.h \
        include/usb2xxx/usb2iic.h \
        include/usb2xxx/usb2ir.h \
        include/usb2xxx/usb2k.h \
        include/usb2xxx/usb2lin.h \
        include/usb2xxx/usb2lin_ex.h \
        include/usb2xxx/usb2nand.h \
        include/usb2xxx/usb2openbus.h \
        include/usb2xxx/usb2ow.h \
        include/usb2xxx/usb2pwm.h \
        include/usb2xxx/usb2rf24.h \
        include/usb2xxx/usb2sniffer.h \
        include/usb2xxx/usb2spi.h \
        include/usb2xxx/usb2uart.h \
        include/usb2xxx/usb_device.h \
        mainwindow.h \
        mychart.h \
        sendmessages.h \
        settings.h \
        standarset.h

FORMS += \
        baseinfo.ui \
        chartdisplay.ui \
        framedisplay.ui \
        mainwindow.ui \
        sendmessages.ui \
        standarset.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



unix:!macx|win32: LIBS += -L$$PWD/lib/ -lECanVci64

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibxl
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibxld
else:unix:!macx: LIBS += -L$$PWD/lib/ -llibxl

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lUSB2XXX
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lUSB2XXXd
else:unix:!macx: LIBS += -L$$PWD/lib/ -lUSB2XXX

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
