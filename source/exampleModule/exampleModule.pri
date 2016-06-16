
QMAKE_CXXFLAGS += -includeexampleModule.hpp

MODULE_PUSH_BACK += "modules.push_back(new exampleModule(this));"


SOURCES += source/exampleModule/exampleInfoTab.cpp \
    source/exampleModule/exampleEditTab.cpp \
    source/exampleModule/exampleModule.cpp

HEADERS  += source/exampleModule/exampleInfoTab.hpp \
    source/exampleModule/exampleEditTab.hpp \
    source/exampleModule/exampleModule.hpp


INCLUDEPATH += source/exampleModule/

FORMS    += source/exampleModule/exampleInfoTab.ui \
    source/exampleModule/exampleEditTab.ui

