#-------------------------------------------------
#
# Project created by QtCreator 2016-07-05T13:32:03
#
#-------------------------------------------------
CONFIG += c++11
CONFIG += no_keywords
#CONFIG += console

QT += core
QT += gui
QT += printsupport
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WoundAnalyzer

#OpenCV
INCLUDEPATH += e:\OpenCV_2.4.13\opencv\localBuild\include
#LIBS += e:\OpenCV_2.4.13\opencv\localBuild\lib\libopencv_core2413.dll.a
#LIBS += e:\OpenCV_2.4.13\opencv\localBuild\lib\libopencv_highgui2413.dll.a
#LIBS += e:\OpenCV_2.4.13\opencv\localBuild\lib\libopencv_imgproc2413.dll.a
#LIBS += e:\OpenCV_2.4.13\opencv\localBuild\lib\libopencv_photo2413.dll.a
#LIBS += e:\OpenCV_2.4.13\opencv\localBuild\lib\libopencv_video2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_core2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_highgui2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_imgproc2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_photo2413.dll.a
LIBS += e:\OpenCV_2.4.13\opencv\localBuild_x32\lib\libopencv_video2413.dll.a

SOURCES += main.cpp\
    Ui/mainwindow.cpp \
    DataBase/databasemanager.cpp \
    Ui/databaseconnectionwidget.cpp \
    Ui/databaseconnectionedititemwidget.cpp \
    Image/imagemanager.cpp \
    Ui/imageinterface.cpp \
    Ui/settingswidget.cpp

HEADERS  += Ui/mainwindow.h \
    DataBase/databasemanager.h \
    Utilities/Logger/logger.h \
    Utilities/timer.h \
    Ui/databaseconnectionwidget.h \
    Ui/databaseconnectioneditwidget.h \
    Ui/databaseconnectionedititemwidget.h \
    Ui/databaseconnectioneditsubitemwidget.h \
    Image/imagemanager.h \
    Ui/imageinterface.h \
    Ui/settingswidget.h \
    Utilities/mathutilities.h \
    DataBase/databasemodel.h

FORMS    += Ui/mainwindow.ui \
    Ui/databaseconnectionwidget.ui \
    Ui/databaseconnectionedititemwidget.ui \
    Ui/settingswidget.ui

DISTFILES += \
    Ui/Icons/Database.png \
    Ui/Icons/Edit.png \
    Ui/Icons/Open.png \
    Ui/Icons/Polygon.png \
    Ui/Icons/Configure.png \
    Ui/Icons/Help.png \
    Ui/Icons/Clear.png \
    Ui/Icons/About.png \
    Ui/Icons/Ruler.png \
    Ui/Icons/Save.png \
    Ui/Icons/Zoom-In.png \
    Ui/Icons/Zoom-Out.png \
    Ui/Icons/ZoomSmall.png \
    Ui/Icons/Transparency.png \
    Ui/Icons/Export.png \
    Ui/Icons/Add.png
