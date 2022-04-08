#-------------------------------------------------
#
# Project created by QtCreator 2022-03-08T16:29:20
#
#-------------------------------------------------

QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Charts
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
        datastruct.cpp \
        dbcontroller.cpp \
        main.cpp \
        mainwindow.cpp \
        mychart.cpp \
        settings.cpp \
        testdisplay.cpp \
        framedisplay.cpp \
        usbcanunion.cpp

HEADERS += \
        ECanVci.h \
#        baseinfo.h \
    baseinfo.h \
        datastruct.h \
        dbcontroller.h \
        include/libxl/IAutoFilterT.h \
        include/libxl/IBookT.h \
        include/libxl/IFilterColumnT.h \
        include/libxl/IFontT.h \
        include/libxl/IFormatT.h \
        include/libxl/ISheetT.h \
        include/libxl/enum.h \
        include/libxl/libxl.h \
        include/libxl/setup.h \
        mainwindow.h \
        framedisplay.h \
        mychart.h \
        settings.h \
        testdisplay.h \
        usbcanunion.h

FORMS += \
        baseinfo.ui \
        testdisplay.ui\
        framedisplay.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/lib/ -lECanVci
INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32: LIBS += -L$$PWD/lib/ -llibxl
INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

DISTFILES += \
    lib/CHUSBDLL.dll \
    lib/CHUSBDLL64.dll \
    lib/ECanVci.dll \
    lib/ECanVci.lib \
    lib/libxl.dll \
    lib/libxl.lib



