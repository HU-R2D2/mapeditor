#include "exampleModule.hpp"

exampleModule::exampleModule(QObject *parent): mapeditorModule(parent)
    {
		std::cout<< "mod con test" << std::endl;
		module_name = "example";
        editTab = new exampleEditTab();
    }
