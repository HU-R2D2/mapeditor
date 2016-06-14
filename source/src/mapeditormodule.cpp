#include "mapeditorModule.hpp"

QWidget *mapeditorModule::getInfoTab() const
{
        return infoTab;
    }

mapeditorModule::mapeditorModule(mapEditor *mapEditor)
    {

    }

QWidget *mapeditorModule::getEditTab() const
    {
        return editTab;
    }
