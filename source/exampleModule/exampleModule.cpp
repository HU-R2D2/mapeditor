#include "exampleModule.hpp"

exampleModule::exampleModule(module_data data, QObject *parent): mapeditorModule(data, parent)
    {
		std::cout<< "mod con test" << std::endl;
		module_name = "example";
        editTab = new exampleEditTab();
    }
