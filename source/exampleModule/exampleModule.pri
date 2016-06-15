#defines that an module is loaded
QMAKE_CXXFLAGS += -DMODULE

QMAKE_CXXFLAGS += -Dhenk

MODULE_PUSH_BACK += "modules.push_back(new exampleModule(ui->graphicsView));"
MODULE_PUSH_BACK += "modules.push_back(new exampleModule(ui->graphicsView));"



#MODULE_CONSTRUCTORS +=

SOURCES += source/exampleModule/exampleInfoTab.cpp \
    source/exampleModule/exampleEditTab.cpp \
    source/exampleModule/exampleModule.cpp

HEADERS  += source/exampleModule/exampleInfoTab.hpp \
    source/exampleModule/exampleEditTab.hpp \
    source/exampleModule/exampleModule.hpp


INCLUDEPATH += source/exampleModule/

FORMS    += source/exampleModule/exampleInfoTab.ui \
    source/exampleModule/exampleEditTab.ui

