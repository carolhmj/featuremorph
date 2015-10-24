#-------------------------------------------------
#
# Project created by QtCreator 2015-10-07T22:03:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FeatureMorph
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    morph.cpp \
    feature.cpp

HEADERS  += mainwindow.h \
    morph.h \
    feature.h

FORMS    += mainwindow.ui
