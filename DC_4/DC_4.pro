#-------------------------------------------------
#
# Project created by QtCreator 2021-07-21T16:38:50
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT       += axcontainer
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DC_4
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

RESOURCES +=  \
        image.qrc
RC_FILE =  \
        ico.rc

SOURCES += \
        baseinfo.cpp \
        chartdisplay.cpp \
        datastruct.cpp \
        deviceunion.cpp \
        framedisplay.cpp \
        main.cpp \
        mainwindow.cpp \
        mychart.cpp \
        settings.cpp \
        sqlprocess.cpp \
        standarset.cpp \
        bootloader.cpp \
        usbcanunion.cpp\
        dbcontroller.cpp\
        scandevrangedialog.cpp \
        dialogaddnode.cpp \
        subcomdialog.cpp
HEADERS += \
        ECanVci.h \
        baseinfo.h \
        chartdisplay.h \
        datastruct.h \
        deviceunion.h \
        framedispaly.h \
        include/libxl/IAutoFilterT.h \
        include/libxl/IBookT.h \
        include/libxl/IFilterColumnT.h \
        include/libxl/IFontT.h \
        include/libxl/IFormatT.h \
        include/libxl/ISheetT.h \
        include/libxl/enum.h \
        include/libxl/libxl.h \
        include/libxl/setup.h \
        include/usb2xxx/can_bootloader.h \
        include/usb2xxx/dialogaddnode.h \
        include/usb2xxx/scandevrangedialog.h \
        include/usb2xxx/usb_device.h \
        mainwindow.h \
        mychart.h \
        settings.h \
        sqlprocess.h \
        standarset.h \
        bootloader.h \
        subcomdialog.h \
        usb_device.h \
        usbcanunion.h\
        scandevrangedialog.h \
        dialogaddnode.h \
        dbcontroller.h\
        can_bootloader.h

FORMS += \
        baseinfo.ui \
        chartdisplay.ui \
        deviceunion.ui \
        framedisplay.ui \
        mainwindow.ui \
        standarset.ui \
        bootloader_ch.ui \
        bootloader_en.ui \
        scandevrangedialog.ui \
        dialogaddnode.ui \
        subcomdialog.ui
		
OTHER_FILES += \
        ico.rc

		
		
		
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




unix:!macx|win32: LIBS += -L$$PWD/lib/ -lECanVci

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx|win32: LIBS += -L$$PWD/lib/ -llibxl

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include


unix:!macx|win32: LIBS += -L$$PWD/lib/ -lUSB2XXX

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

