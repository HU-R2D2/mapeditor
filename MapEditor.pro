#----------------------------------------------
#
# Project created by QtCreator 2016-03-29T14:23:54
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

RC_FILE = source/mapeditor.rc
ICON = source/mapeditor.icns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapEditor
TEMPLATE = app

#include path for the .pri file to load the moduels
#include(source/exampleModule/exampleModule.pri)
include(source/PathfinderModule/PathfinderModule.pri)

SOURCES +=    source/src/mainwindow.cpp \
    source/src/mapEditor.cpp \
    source/src/mapView.cpp \
    source/src/viewScene.cpp \
    source/src/mapeditormodule.cpp \
    ../adt/source/src/Length.cpp \
    ../adt/source/src/Translation.cpp \
    ../adt/source/src/Box.cpp \
    ../adt/source/src/Coordinate.cpp \
    ../map/source/src/MapInterface.cpp \
    ../map/source/src/ArrayBoxMap.cpp \
    ../map/source/src/RStarMap.cpp \
    ../map/source/src/BoxMap.cpp \
    source/src/main.cpp


HEADERS  += source/include/mainwindow.hpp \
    source/include/mapEditor.hpp \
    source/include/mapView.hpp \
    source/include/viewScene.hpp \
    ../adt/source/include/Length.hpp \
    ../adt/source/include/Translation.hpp \
    ../adt/source/include/Box.hpp \
    ../adt/source/include/Coordinate.hpp \
    ../map/source/include/MapInterface.hpp \
    ../map/source/include/BoxMap.hpp \
    ../map/source/include/DefaultBoxMap.hpp \
    source/include/mapeditormodule.hpp \
    build/ui_mainwindow.h

INCLUDEPATH += ../adt/source/include/
INCLUDEPATH += ../map/source/include/
INCLUDEPATH += source/include/
INCLUDEPATH += source/PathfinderModule/

linux-g++ {
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtGui
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtCore
    QMAKE_CXXFLAGS += --std=gnu++11
}

FORMS    += source/mainwindow.ui

QMAKE_CXXFLAGS += \""-DMODULE_CONSTRUCTORS $${MODULE_PUSH_BACK}"\"
