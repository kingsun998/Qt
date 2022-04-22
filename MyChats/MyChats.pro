#-------------------------------------------------
#
# Project created by QtCreator 2022-01-10T21:59:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyChats
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
        test.cpp

HEADERS += \
        \ \
    include/event2/buffer.h \
    include/event2/buffer_compat.h \
    include/event2/bufferevent.h \
    include/event2/bufferevent_compat.h \
    include/event2/bufferevent_ssl.h \
    include/event2/bufferevent_struct.h \
    include/event2/dns.h \
    include/event2/dns_compat.h \
    include/event2/dns_struct.h \
    include/event2/event-config.h \
    include/event2/event.h \
    include/event2/event_compat.h \
    include/event2/event_struct.h \
    include/event2/http.h \
    include/event2/http_compat.h \
    include/event2/http_struct.h \
    include/event2/keyvalq_struct.h \
    include/event2/listener.h \
    include/event2/rpc.h \
    include/event2/rpc_compat.h \
    include/event2/rpc_struct.h \
    include/event2/tag.h \
    include/event2/tag_compat.h \
    include/event2/thread.h \
    include/event2/util.h \
    include/event2/visibility.h \
     mainwindow.h \
    test.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/lib/ -lWS2_32
win32: LIBS += -L$$PWD/lib/ -levent
win32: LIBS += -L$$PWD/lib/ -levent_core
win32: LIBS += -L$$PWD/lib/ -levent_extra

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

DISTFILES += \
    lib/event.dll \
    lib/event.lib \
    lib/event_core.dll \
    lib/event_core.lib \
    lib/event_extra.dll \
    lib/event_extra.lib \
    lib/WS2_32.Lib

