#-------------------------------------------------
#
# Project created by QtCreator 2015-06-27T22:18:28
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Net
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
    myudp.cpp \
    commonhelper.cpp

HEADERS  += mainwindow.h \
    myudp.h \
    commonhelper.h

FORMS    += mainwindow.ui
