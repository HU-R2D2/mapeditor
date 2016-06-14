#ifndef MAPEDITORMODULE_HPP
#define MAPEDITORMODULE_HPP

#include "mapEditor.hpp"

class mapeditorModule{

private:

    //edit tab
    QWidget * editTab;

    //info tab
    QWidget * infoTab;

    mapEditor * mapEditorPointer;


public:

    QWidget *getEditTab() const;
    QWidget *getInfoTab() const;

    mapeditorModule(mapEditor * mapEditor);
    };

#endif // MAPEDITORMODULE_HPP


