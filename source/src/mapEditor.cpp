#include "../include/mapEditor.hpp"


mapEditor::mapEditor(QWidget *parent) :
    mapView(parent)
    {
    }

void mapEditor::removeTile(){
    editTile(QString("Unknown"));
}

void mapEditor::editTile(QString type){

    //set new boxInfo
    r2d2::BoxInfo info;
    if(type == "Navigatable"){
        info = r2d2::BoxInfo(false, false, true);
    }else if(type == "Obstacle"){
        info = r2d2::BoxInfo(true, false, false);
    }else if(type == "Unknown"){
        info = r2d2::BoxInfo(false, true, false);
    }else{
        info = r2d2::BoxInfo(true, false, true);
    }

    editBuffer.push_back(std::pair<r2d2::Box, r2d2::BoxInfo>(selectedBox, info));
    map->set_box_info(selectedBox, info);

    std::cout << "Saved boxes: " << editBuffer.size() << std::endl;
    drawMap();
}
