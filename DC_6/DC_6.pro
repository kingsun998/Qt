#-------------------------------------------------
#
# Project created by QtCreator 2021-07-21T16:38:50
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT       += sql
QT       += network
QT       += mqtt
QT       += axcontainer
QT       += network
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DC_6
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
QMAKE_CXXFLAGS+= -std=c++11
RESOURCES +=  \
        image.qrc \
        image.qrc
RC_FILE =  \
        ico.rc

SOURCES += \
        autopack_step_one.cpp \
        autopack_step_three.cpp \
        autopack_step_two.cpp \
        autopacking.cpp \
        baseinfo.cpp \
        bootloader.cpp \
        datastruct.cpp \
        dbcontroller.cpp \
        dialogaddnode.cpp \
        framedisplay.cpp \
        httpserver.cpp \
        main.cpp \
        mainwindow.cpp \
        mqttcommunication.cpp \
        mqttdatastruct.cpp \
        multipleunion.cpp \
        mychart.cpp \
        mytcpsocket.cpp \
        scandevrangedialog.cpp \
        scangun.cpp \
        settings.cpp \
        subcomdialog.cpp \
        test.cpp \
        testdisplay.cpp \
        usbcanunion.cpp \
        standarset.cpp \
        wzserialport.cpp
#        sqlprocess.cpp \

HEADERS += \
        ECanVci.h \
        autopack_step_one.h \
        autopack_step_three.h \
        autopack_step_two.h \
        autopacking.h \
        baseinfo.h \
        bootloader.h \
        datastruct.h \
        dbcontroller.h \
        framedispaly.h \
        httpserver.h \
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
        mqttcommunication.h \
        mqttdatastruct.h \
        multipleunion.h \
        mychart.h \
        mytcpsocket.h \
        scangun.h \
        settings.h \
        subcomdialog.h \
        test.h \
        testdisplay.h \
        usbcanunion.h \
        standarset.h \
        wzserialport.h
#        sqlprocess.h \

FORMS += \
        autopack_step_one.ui \
        autopack_step_three.ui \
        autopack_step_two.ui \
        autopacking.ui \
        baseinfo.ui \
        framedisplay.ui \
        mainwindow.ui \
        multipleunion.ui \
        scangun.ui \
        subcomdialog.ui \
        bootloader_ch.ui \
        bootloader_en.ui \
        scandevrangedialog.ui \
        dialogaddnode.ui \
        test.ui \
        testdisplay.ui \
        standarset.ui
		
OTHER_FILES += \
        ico.rc

		
		
		
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    App(1).bin \
    App_0804.bin \
    easyicon_net.ico \
    images/8.png \
    images/AutoBaudRate.ico \
    images/CAN.ico \
    images/Going down two bars 128.png \
    images/Going down two bars 256.png \
    images/Going down two bars 32.png \
    images/Going down two bars 512.png \
    images/Going down two bars 64.png \
    images/atom.png \
    images/calculator.ico \
    images/chip.ico \
    images/chip.png \
    images/easyicon_net.png \
    images/gif_48_001.gif \
    images/gif_48_002.gif \
    images/gif_48_003.gif \
    images/gif_48_004.gif \
    images/gif_48_005.gif \
    images/gif_48_006.gif \
    images/gif_48_007.gif \
    images/gif_48_008.gif \
    images/gif_48_009.gif \
    images/gif_48_010.gif \
    images/gif_48_011.gif \
    images/gif_48_012.gif \
    images/gif_48_013.gif \
    images/gif_48_014.gif \
    images/gif_48_015.gif \
    images/gif_48_016.gif \
    images/gif_48_017.gif \
    images/gif_48_018.gif \
    images/gif_48_019.gif \
    images/gif_48_020.gif \
    images/gif_48_021.gif \
    images/gif_48_022.gif \
    images/gif_48_023.gif \
    images/gif_48_024.gif \
    images/gif_48_025.gif \
    images/gif_48_026.gif \
    images/gif_48_027.gif \
    images/gif_48_028.gif \
    images/gif_48_029.gif \
    images/gif_48_030.gif \
    images/gif_48_031.gif \
    images/gif_48_032.gif \
    images/gif_48_033.gif \
    images/gif_48_034.gif \
    images/gif_48_035.gif \
    images/gif_48_036.gif \
    images/gif_48_037.gif \
    images/gif_48_038.gif \
    images/gif_48_039.gif \
    images/gif_48_040.gif \
    images/gif_48_041.gif \
    images/gif_48_042.gif \
    images/gif_48_043.gif \
    images/gif_48_044.gif \
    images/gif_48_045.gif \
    images/gif_48_046.gif \
    images/gif_48_047.gif \
    images/gif_48_048.gif \
    images/gif_48_049.gif \
    images/gif_48_050.gif \
    images/gif_48_051.gif \
    images/gif_48_052.gif \
    images/gif_48_053.gif \
    images/gif_48_054.gif \
    images/gif_48_055.gif \
    images/gif_48_056.gif \
    images/gif_48_057.gif \
    images/gif_48_058.gif \
    images/gif_48_059.gif \
    images/gif_48_060.gif \
    images/gif_48_061.gif \
    images/gif_48_062.gif \
    images/gif_48_063.gif \
    images/gif_48_064.gif \
    images/gif_48_065.gif \
    images/gif_48_066.gif \
    images/gif_48_067.gif \
    images/gif_48_068.gif \
    images/gif_48_069.gif \
    images/gif_48_070.gif \
    images/gif_48_071.gif \
    images/gif_48_072.gif \
    images/gif_48_073.gif \
    images/gif_48_074.gif \
    images/gif_48_075.gif \
    images/gif_48_076.gif \
    images/gif_48_077.gif \
    images/gif_48_078.gif \
    images/gif_48_079.gif \
    images/gif_48_080.gif \
    images/gif_48_081.gif \
    images/gif_48_082.gif \
    images/gif_48_083.gif \
    images/gif_48_084.gif \
    images/gif_48_085.gif \
    images/gif_48_086.gif \
    images/gif_48_087.gif \
    images/gif_48_088.gif \
    images/gif_48_089.gif \
    images/gif_48_090.gif \
    images/gif_48_091.gif \
    images/gif_48_092.gif \
    images/gif_48_093.gif \
    images/gif_48_094.gif \
    images/gif_48_095.gif \
    images/gif_48_096.gif \
    images/gif_48_097.gif \
    images/gif_48_098.gif \
    images/gif_48_099.gif \
    images/gif_48_100.gif \
    images/gif_48_101.gif \
    images/gif_48_102.gif \
    images/gif_48_103.gif \
    images/gif_48_104.gif \
    images/gif_48_105.gif \
    images/gif_48_106.gif \
    images/gif_48_107.gif \
    images/gif_48_108.gif \
    images/gif_48_109.gif \
    images/gif_48_110.gif \
    images/hwinfo.png \
    images/open.png \
    images/processor.png \
    images/save.png \
    lib/CHUSBDLL.dll \
    lib/ECanVci.dll \
    lib/ECanVci.lib \
    lib/USB2XXX.dll \
    lib/USB2XXX.lib \
    lib/libxl.dll \
    lib/libxl.lib



win32: LIBS += -L$$PWD/lib/ -llibxl

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32: LIBS += -L$$PWD/lib/ -lECanVci

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32: LIBS += -L$$PWD/lib/ -lUSB2XXX

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include


