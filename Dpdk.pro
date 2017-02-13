#-------------------------------------------------
#
# Project created by QtCreator 2016-09-02T15:09:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = Dpdk
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fenset.cpp \
    fensetip.cpp \
    fensetmac.cpp \
    fensetsrcmac.cpp \
    pipe.cpp \
    portwindows.cpp \
    clientthread.cpp \
    connexion.cpp \
    senderthread.cpp \
    datadispatcher.cpp \
    freqwindow.cpp

HEADERS  += mainwindow.h \
    fenset.h \
    fensetip.h \
    fensetmac.h \
    fensetsrcmac.h \
    pipe.h \
    portwindows.h \
    clientthread.h \
    connexion.h \
    senderthread.h \
    datadispatcher.h \
    freqwindow.h

FORMS    += mainwindow.ui \
    fenset.ui \
    fensetip.ui \
    fensetmac.ui \
    fensetsrcmac.ui \
    portwindows.ui \
    connexion.ui \
    freqwindow.ui
