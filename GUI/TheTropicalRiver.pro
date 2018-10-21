#-------------------------------------------------
#
# Project created by QtCreator 2017-11-10T09:56:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport printsupport

DESTDIR=build
OBJECTS_DIR=build
MOC_DIR=build
UI_DIR=build


TARGET = TheTropicalRiver 
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    MsgHandler.cpp \
    Camera_FLIR.cpp \
    Frame_Writer.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    MsgHandler.h \
    Camera_FLIR.h \
    Frame_Writer.h \
    qcustomplot.h

FORMS    += mainwindow.ui

DISTFILES += \
    output.css \
    Settings.conf

# === Platform-specific libraries ==========================================

# --- LINUX
unix:!macx: LIBS += -L/usr/local/Spinnaker/lib -lSpinnaker
unix:!macx: INCLUDEPATH += /usr/include/spinnaker
