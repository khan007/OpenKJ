#-------------------------------------------------
#
# Project created by QtCreator 2013-09-20T09:00:16
#
#-------------------------------------------------

QT       += core gui sql multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BreakMusic
TEMPLATE = app

#win32: CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    songdbtablemodel.cpp \
    playlisttablemodel.cpp \
    khipc.cpp \
    sourcedirtablemodel.cpp \
    databasedialog.cpp \
    databaseupdatethread.cpp \
    fader.cpp \
    bmipcserver.cpp \
    bmplaylist.cpp \
    bmsong.cpp \
    bmsourcedir.cpp \
    bmsettings.cpp

HEADERS  += mainwindow.h \
    songdbtablemodel.h \
    playlisttablemodel.h \
    khipc.h \
    sourcedirtablemodel.h \
    databasedialog.h \
    databaseupdatethread.h \
    fader.h \
    bmipcserver.h \
    bmplaylist.h \
    bmsong.h \
    bmsourcedir.h \
    bmsettings.h

FORMS    += mainwindow.ui \
    databasedialog.ui

RESOURCES += \
    resources.qrc

unix: QT_CONFIG -= no-pkg-config
unix: CONFIG += link_pkgconfig

unix: PKGCONFIG += taglib_c

win32: INCLUDEPATH += "C:\taglib\include\taglib"
win32: LIBS += -L"C:\taglib\taglib\taglib\Release" -ltag
#win32: LIBS += -L"$$_PRO_FILE_PWD_/taglib-win32/lib" -ltag
