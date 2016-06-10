#include "../include/mapEditor.hpp"


mapEditor::mapEditor(QWidget *parent) :
    mapView(parent)
    {
        edited = false;
    }

void mapEditor::removeTile(){
    editTile(QString("Unknown"));
}

void mapEditor::editTile(QString type){

    //set new boxInfo
    r2d2::BoxInfo info;
    if(type == "Navigatable"){
        info = r2d2::BoxInfo(false, true, false);
    }else if(type == "Obstacle"){
        info = r2d2::BoxInfo(true, false, false);
    }else if(type == "Unknown"){
        info = r2d2::BoxInfo(false, false, true);
    }else{
        info = r2d2::BoxInfo(true, true, false);
    }

    //store
    map->set_box_info(selectedBox, info);
    drawSingleBox(selectedBox, info);

}


