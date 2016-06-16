#if all setings are correct you only need to include this .pri file in the main .pro file
#example:
#include(<path_to_this_file>/<template_module>.pri)


#set here the main template_module class without the <> brackets and no space between the -include and the file
QMAKE_CXXFLAGS += -includePathfinderModule.hpp


#replace<template_module> with the name of the template_module
MODULE_PUSH_BACK += "modules.push_back(new PathfinderModule{data, this});"


#<file_path> is relative to .pro file of mapeditor
SOURCES += source/PathfinderModule/PathfinderEditTab.cpp \
    source/PathfinderModule/PathfinderInfoTab.cpp \
    source/PathfinderModule/PathfinderModule.cpp \
    ../pathfinding/source/src/AStarPathFinder.cpp \
    ../sharedobjects/source/include/LockingSharedObject.hpp

HEADERS  += source/PathfinderModule/PathfinderEditTab.hpp \
            source/PathfinderModule/PathfinderInfoTab.hpp \
            source/PathfinderModule/PathfinderModule.hpp \
            ../pathfinding/source/include/PathFinder.hpp \
            ../pathfinding/source/include/AStarPathFinder.hpp \
            ../sharedobjects/source/include/SharedObject.hpp \
            ../sharedobjects/source/include/LockingSharedObject.hpp

#path of files that are included
INCLUDEPATH += source/PathfinderModule/
INCLUDEPATH += ../pathfinding/source/include/
INCLUDEPATH += ../sharedobjects/source/include/

FORMS    += source/PathfinderModule/PathfinderInfoTab.ui \
            source/PathfinderModule/PathfinderEditTab.ui

