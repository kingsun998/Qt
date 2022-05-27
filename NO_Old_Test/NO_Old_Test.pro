#-------------------------------------------------
#
# Project created by QtCreator 2022-05-07T10:09:18
#
#-------------------------------------------------

QT       += core gui network mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NO_Old_Test
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
        main.cpp \
        mainwindow.cpp \
        messagedisplay.cpp \
        mode_scan.cpp \
        mode_sending.cpp \
        mode_settings.cpp \
        mode_upload.cpp \
        release/moc_mainwindow.cpp \
        release/moc_messagedisplay.cpp \
        release/moc_usbcanunion.cpp \
        settings.cpp \
        usbcanunion.cpp \
        utils.cpp

HEADERS += \
        include/canframe.h \
        include/config.h \
        include/typedef.h \
        include/zlgcan.h \
        mainwindow.h \
        messagedisplay.h \
        mode_scan.h \
        mode_sending.h \
        mode_settings.h \
        mode_upload.h \
        release/moc_predefs.h \
        settings.h \
        usbcanunion.h \
        utils.h

FORMS += \
        mainwindow.ui \
        messagedisplay.ui \
        mode_scan.ui \
        mode_sending.ui \
        mode_settings.ui \
        mode_upload.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    configuration.ini \
    images/scan.jpeg \
    images/sending.jpeg \
    images/settings.jpg \
    images/upload.jpeg \
    lib/zlgcan.dll \
    lib/zlgcan.lib



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lzlgcan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lzlgcand



INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
