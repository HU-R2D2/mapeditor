#include "../include/mapEditor.hpp"


mapEditor::mapEditor(QWidget *parent) :
    mapView(parent)
    {
        //saveBuffer = RectInfo_from_map_using_tiles(map, 50, 50);
    }

void mapEditor::createTile(){
    //create new Tile

}

void mapEditor::removeTile(){
    //editTile("Unknown");
}

void mapEditor::editTile(QString type){

    //set new boxInfo
    r2d2::BoxInfo info;
    if(type == "Navigatable"){
        info = r2d2::BoxInfo(false, false, true);
    }else if(type == "Obstacle"){
        info = r2d2::BoxInfo(true, false, false);
    }else if(type == "Mixed"){
        info = r2d2::BoxInfo(true, true, true);
    }else{
        info = r2d2::BoxInfo(false, false, false);
    }

    //store

    for(r2d2::Box tile: selectedBoxes){
        saveBuffer.push_back(std::pair<r2d2::Box, r2d2::BoxInfo>(tile, info));
        map->set_box_info(tile, info);
        std:: cout << "write: " << saveBuffer.size() << std::endl;
    }
    scene->deleteSelectedItems();
    deselectTiles();
    std::cout << "Saved Tiles: " << saveBuffer.size() << std::endl;
}

void mapEditor::displayEdit(){
    int tileSize=10;
    scene->clearSelection();

    while(!saveBuffer.empty()){
        std::pair<r2d2::Box, r2d2::BoxInfo> box = saveBuffer.back();
        saveBuffer.pop_back();
        //std:: cout << "draw: " << saveBuffer.size() << std::endl;
        //fflush(stdout);
        drawBox(box.first, tileSize);
    }
    this->setSelectable(true);
}
