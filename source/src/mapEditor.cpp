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
        info = r2d2::BoxInfo(false, false, true);
    }else if(type == "Obstacle"){
        info = r2d2::BoxInfo(true, false, false);
    }else if(type == "Unknown"){
        info = r2d2::BoxInfo(false, true, false);
    }else{
        info = r2d2::BoxInfo(true, false, true);
    }

    //store
        std::cout << "i worked my way up here" << std::endl;
        fflush(stdout);
        editBuffer.push_back(std::pair<r2d2::Box, r2d2::BoxInfo>(selectedBox, info));
        map->set_box_info(selectedBox, info);
        //edited = false;
        std:: cout << "write: " << editBuffer.size() << std::endl;

        scene->deleteSelectedItems();
        deselectTiles();
        std::cout << "Saved Tiles: " << editBuffer.size() << std::endl;

}

void mapEditor::displayEdit(){
    int tileSize=10;
    scene->clearSelection();

    while(!editBuffer.empty()){
        std::pair<r2d2::Box, r2d2::BoxInfo> box = editBuffer.back();
        editBuffer.pop_back();
        drawBox(box.first, tileSize, false);
    }
    this->setSelectable(true);
}
