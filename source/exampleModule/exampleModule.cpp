#include "exampleModule.hpp"

exampleModule::exampleModule(mapEditor * sad, QObject *parent): mapeditorModule(sad,parent)
    {
		std::cout<< "mod con test" << std::endl;
		module_name = "example";
        editTab = new exampleEditTab();
    }
