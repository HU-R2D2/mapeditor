#if all setings are correct you only need to include this .pri file in the main .pro file
#example:
#include(<path_to_this_file>/<template_module>.pri)


#set here the main template_module class without the <> brackets and no space between the -include and the file
QMAKE_CXXFLAGS += -include<template_module.hpp>


#replace<template_module> with the name of the template_module
MODULE_PUSH_BACK += "modules.push_back(new <template_module>(this));"


#<file_path> is relative to .pro file of mapeditor
#include here all files that are needed by your module
#note: there is a bug where including the same file causes qmake to crash, 
# so dont include files that are already included by map editor itself or other depencencies.

SOURCES += <file_path>/<template_edit_tab>.cpp \
    <file_path>/<template_info_tab>.cpp \
    <file_path>/<template_module>.cpp

HEADERS  += <file_path>/<template_edit_tab>.hpp \
            <file_path>/<template_info_tab>.hpp \
            <file_path>/<template_module>.hpp

#path of all header files that are included in your module that are not included by the mapeditor itself.
INCLUDEPATH += <file_path>/

FORMS    += <file_path>/<template_info_tab>.ui \
            <file_path>/<template_edit_tab>.ui

