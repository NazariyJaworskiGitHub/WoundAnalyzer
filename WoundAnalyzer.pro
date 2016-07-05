#-------------------------------------------------
#
# Project created by QtCreator 2016-07-05T13:32:03
#
#-------------------------------------------------
CONFIG += c++11
CONFIG += no_keywords

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WoundAnalyzer
TEMPLATE = app


#OpenCV
INCLUDEPATH += e:\OpenCV_2.4.13\opencv\build\include
LIBS += e:\OpenCV_2.4.13\opencv\localBuild\lib\libopencv_core2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild\lib\libopencv_highgui2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild\lib\libopencv_imgproc2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild\lib\libopencv_video2413.dll.a

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
