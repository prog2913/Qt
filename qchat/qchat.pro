#-------------------------------------------------
#
# Project created by QtCreator 2017-05-05T00:05:33
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qchat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutwindow.cpp \
    networkfile.cpp

HEADERS  += mainwindow.h \
    aboutwindow.h \
    networkfile.h

FORMS    += mainwindow.ui \
    aboutwindow.ui

RESOURCES += \
    resource.qrc

