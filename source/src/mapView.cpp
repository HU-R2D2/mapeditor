//! \addtogroup 0008 Mapviewer/-editor
//! \brief Functions for Viewing and navigating a loaded map
//!
//! this class extends Graphicsview (the qt ui viewer class).
//! it implemts a set of functions zoom, rotate and move trough a map.
//! it enables the selection of boxes and forms a super class for mapeditor.
//! this class initiliazis the viewscene (the scene that is displayed).
//!
//! \file   mapEditor.hpp
//! \author Jop van Buuren, 1658718
//!         Daniel Klomp, 1661521
//!         Koen de Guijter, 1671103
//!         Jasper Schoenmaker, 1661818
//! \date   Created: 30-03-2016
//! \date   Last Modified: 10-06-2016
//! \brief  Header for mapView
//!
//! This is the header file for mapView
//!
//! \copyright Copyright © 2016, HU University of Applied Sciences Utrecht.
//! All rights reserved.
//!
//! License: newBSD
//!
//! Redistribution and use in source and binary forms,
//! with or without modification, are permitted provided that
//! the following conditions are met:
//! - Redistributions of source code must retain the above copyright notice,
//!   this list of conditions and the following disclaimer.
//! - Redistributions in binary form must reproduce the above copyright notice,
//!   this list of conditions and the following disclaimer in the documentation
//!   and/or other materials provided with the distribution.
//! - Neither the name of the HU University of Applied Sciences Utrecht
//!   nor the names of its contributors may be used to endorse or promote
//!   products derived from this software without specific prior written
//!   permission.
//!
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//! "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
//! BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
//! AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//! IN NO EVENT SHALL THE HU UNIVERSITY OF APPLIED SCIENCES UTRECHT
//! BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//! CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//! PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
//! OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
//! WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
//! OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//! EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ~< HEADER_VERSION 2016 04 12 >~

#include "mapView.hpp"
#include "DefaultBoxMap.hpp"
#include <iostream>
#include <QMouseEvent>
#include <QScrollBar>
#include <QInputEvent>
#include <QEnterEvent>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QtGui>
#include <QMutex>
#include <QRect>

QMutex EventRecursion;

MapView::MapView(QWidget *parent):
    QGraphicsView(parent),
    windowWidth(1000),
    windowHeight(1000)
{  
    tileColors[MapTypes::TileType::EMPTY]=Qt::green;
    tileColors[MapTypes::TileType::BLOCKED]=Qt::red;
    tileColors[MapTypes::TileType::MIXED]=Qt::yellow;
    tileColors[MapTypes::TileType::UNKNOWN]=Qt::gray;

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    scene = new ViewScene;
    QPalette p = scene->palette();

    p.setColor(QPalette::Highlight, Qt::red);

    scene->setPalette(p);

    std::cout << "new Viewer with size: " << windowWidth << " x " <<
                 windowHeight << std::endl;

    scene->setSceneRect( 0, 0, windowWidth, windowHeight);
    setScene(scene);
    show();

    //set default zoom
    resetScale();
    scene->installEventFilter(this);
    verticalScrollBar()->installEventFilter(this);
    horizontalScrollBar()->installEventFilter(this);
    setMouseTracking(true);

    recenterMap();
}

MapView::~MapView(){
    delete scene;
}

void MapView::setSelectable(bool state){
    for(QGraphicsItem * item: items()){
        if(scene->isTile(item)){
            item->setFlag(QGraphicsItem::ItemIsSelectable, state);
        }
    }
}

void MapView::increaseZoom(){
    scaleSize += zoomSpeed;
    if(scaleSize > maxZoom){
        scaleSize = maxZoom;
    }

    updateTransform();
}

void MapView::decreaseZoom(){
    scaleSize -= zoomSpeed;
    if(scaleSize < minZoom){
        scaleSize = minZoom;
    }
    updateTransform();
}

int MapView::getScale(){
    //give range 0-100%
    return scaleSize * 100 / maxZoom;
}

void MapView::resetScale(){
    scaleSize = (maxZoom / 2);
    updateTransform();
}

void MapView::increaseRotation(int inc){
    rotation = (rotation + inc) % 360;
    updateTransform();
}

void MapView::decreaseRotation(int dec){
    if(rotation == 0){
        rotation = 360 - dec;
    }else{
        rotation -= dec;
    }
    updateTransform();
}

int MapView::getRotation(){
    return rotation;
}

void MapView::resetRotation(){
    rotation = 0;
    updateTransform();
}

void MapView::updateTransform(){
    resetTransform();
    rotate(rotation);
    scale(scaleSize, scaleSize);
    drawMap();
}

void MapView::set_z_bottom(float value)
{
    z_bottom = value;
}

void MapView::set_z_top(float value){
    z_top = value;
}


void MapView::setZoomSpeed(qreal speed){
    zoomSpeed = speed;
}

void MapView::deselectTiles(){
    for(QGraphicsItem * tile : scene->selectedItems()){
        tile->setSelected(false);
    }
}

void MapView::updateSelection(){
    QRectF newBoxArea = scene->selectionArea().boundingRect();
    r2d2::Box box(scene->qpoint_2_box_coordinate(
                  newBoxArea.bottomLeft(), z_bottom),
                  scene->qpoint_2_box_coordinate(newBoxArea.topRight(), z_top)
                  );

    selectedBox = box;
    drawMap();
}

selectionData MapView::getSelectionData(){
    QRectF selection = scene->selectionArea().boundingRect();
    r2d2::Box mapBox(
                scene->qrect_2_box_coordinate(
                    selection,
                    z_bottom,
                    z_top)
                );

    r2d2::BoxInfo bi = map->get_box_info(mapBox);
    switch(getTileType(bi)){
        case MapTypes::TileType::EMPTY:
            selData.type = QString("Navigatable");
            break;
        case MapTypes::TileType::BLOCKED:
            selData.type = QString("Blocked");
            break;
        case MapTypes::TileType::MIXED:
            selData.type = QString("Mixed");
            break;
        default:
            selData.type = QString("Unknown");
            break;
    }

    r2d2::Coordinate topLeft(
        scene->qpoint_2_box_coordinate(selection.topLeft(),z_bottom)
    );

    r2d2::Coordinate bottomRight(
        scene->qpoint_2_box_coordinate(selection.bottomRight(),z_top)
    );

    selData.xtop = topLeft.get_x()/r2d2::Length::CENTIMETER;
    selData.ytop = topLeft.get_y()/r2d2::Length::CENTIMETER;
    selData.xbottom = bottomRight.get_x()/r2d2::Length::CENTIMETER;
    selData.ybottom = bottomRight.get_y()/r2d2::Length::CENTIMETER;
    selData.width = selection.width();
    selData.height = selection.height();
    selData.ztop = mapBox.get_top_right().get_z()/r2d2::Length::CENTIMETER;
    selData.zbottom = mapBox.get_bottom_left().get_z()/r2d2::Length::CENTIMETER;
    return selData;
}

bool MapView::event(QEvent *event){
    switch(event->type()){
        case QEvent::KeyPress:{
            QKeyEvent * ke = static_cast<QKeyEvent*>(event);
            if(ke->key() == Qt::Key_Down){
                int val = verticalScrollBar()->value();
                verticalScrollBar()->setValue(
                            val+scrollStepSize);
                drawMap();
            }
            else if(ke->key() == Qt::Key_Up){
                int val = verticalScrollBar()->value();
                verticalScrollBar()->setValue(
                            val-scrollStepSize);
                drawMap();
            }
            else if(ke->key() == Qt::Key_Right){
                int val = horizontalScrollBar()->value();
                horizontalScrollBar()->setValue(
                            val+scrollStepSize);
                drawMap();
            }
            else if(ke->key() == Qt::Key_Left){
                int val = horizontalScrollBar()->value();
                horizontalScrollBar()->setValue(
                            val-scrollStepSize);
                drawMap();
            }
            break;}
        default:
            break;
    }
    return QGraphicsView::event(event);
}

void MapView::checkSceneBorder(){
    QPointF startPoint = mapToScene(QPoint(0,0));
    QPointF endPoint = mapToScene(QPoint(width(),height()));
    int stepSize = endPoint.x()-startPoint.x();

    if(startPoint.x() < stepSize && EventRecursion.tryLock()){
        //std::cout << "too close to startPoint.x" << std::endl;
        QPointF center = mapToScene(viewport()->rect().center());
        scene->addOriginOffset(stepSize,0);
        centerOn(QPointF(stepSize,0)+center);//causes an event (recursion)
        EventRecursion.unlock();
    }
    if(startPoint.y() < stepSize && EventRecursion.tryLock()){
        //std::cout << "too close to startPoint.y" << std::endl;
        QPointF center = mapToScene(viewport()->rect().center());
        scene->addOriginOffset(0,stepSize);
        centerOn(QPointF(0,stepSize)+center);//causes an event (recursion)
        EventRecursion.unlock();
    }
    if(endPoint.x() > (scene->width() - stepSize)){
        //std::cout << "too close to endPoint.x" << std::endl;
        scene->setSceneRect(0,0,scene->width() + stepSize, scene->height());
        scene->drawAxes();
    }

    if(endPoint.y() > (scene->height() - stepSize)){
        //std::cout << "too close to endPoint.y" << std::endl;
        scene->setSceneRect(0,0,scene->width(), scene->height()+stepSize);
        scene->drawAxes();
    }
}

void MapView::loadMapFile(string file) {
    map = new r2d2::DefaultBoxMap;
    map->load(file);
    recenterMap();
    drawMap();
}

void MapView::saveMapFile(std::string file){
    map->save(file);

}

bool MapView::eventFilter(QObject * object, QEvent * event){
    checkSceneBorder();
    switch(event->type()){
        case QEvent::Wheel:
           if (object == verticalScrollBar()){//catch
               QWheelEvent* we = static_cast<QWheelEvent*>(event);
               int num = we->delta();
               if(num < 0){
                   decreaseZoom();
               }else{
                   increaseZoom();
               }
               return true;
               }
            else if(object == horizontalScrollBar()){
                return true;}
            break;
        case QEvent::GraphicsSceneMouseRelease:{
               QGraphicsSceneMouseEvent * gsme =
                       static_cast<QGraphicsSceneMouseEvent*>(event);
               if(dragMode() == QGraphicsView::RubberBandDrag
                    && (gsme->button() == Qt::MouseButton::RightButton)
               ){
                   scene->drawSelection();
               }
               return true;
               break;
           }
        default:
            break;
    }
    return false;
}

MapTypes::TileType MapView::getTileType(r2d2::BoxInfo & tileInfo){
    bool nav = tileInfo.get_has_navigatable();
    bool obs = tileInfo.get_has_obstacle();
    bool unk = tileInfo.get_has_unknown();
    if(nav && !obs && !unk){
        return MapTypes::TileType::EMPTY;}
    else if (!nav && obs && !unk){
            return MapTypes::TileType::BLOCKED;}
    else if (!nav && !obs && unk){
            return MapTypes::TileType::UNKNOWN;}
    else{return MapTypes::TileType::MIXED;}
}

void MapView::drawMap(){
    scene->clear();
    QPointF topLeft(mapToScene(QPoint(0,height())));
    QPointF bottemRight(mapToScene(QPoint(width(),0)));

    r2d2::Box screenToSceneCoordinate(
        scene->qrect_2_box_coordinate(
            QRectF(topLeft, bottemRight),
            z_bottom,
            z_top
        )
    );

    //This renders only what is in the view after scrolling
    std::vector<std::pair<r2d2::Box, r2d2::BoxInfo>> boxesOnScreen =
            map->get_intersecting(screenToSceneCoordinate);

    for(std::pair<r2d2::Box, r2d2::BoxInfo> pair: boxesOnScreen){
        drawSingleBox(pair.first, pair.second);
    }
    scene->drawAxes();
}

void MapView::drawSingleBox(r2d2::Box box, r2d2::BoxInfo info){
    const r2d2::Coordinate bottemLeft{
        box.get_bottom_left().get_x() ,
        box.get_bottom_left().get_y() ,
        r2d2::Length::CENTIMETER * z_bottom
    };

    const r2d2::Coordinate topRight{
        box.get_top_right().get_x() ,
        box.get_top_right().get_y() ,
        r2d2::Length::CENTIMETER * z_top
    };

    r2d2::Box tempbox(bottemLeft,topRight);
    scene->drawTile(tempbox, tileColors[getTileType(info)]);
}

void MapView::recenterMap(){
    //X, Y and Z points are set to 0
    centerOn(scene->box_coordinate_2_qpoint(
        r2d2::Coordinate(
            0*r2d2::Length::CENTIMETER,
            0*r2d2::Length::CENTIMETER,
            0*r2d2::Length::CENTIMETER)
        )
    );
}

void MapView::mouseReleaseEvent(QMouseEvent* event){
    drawMap();
    QGraphicsView::mouseReleaseEvent(event);
}

void MapView::emptyMap(){
    map = new r2d2::DefaultBoxMap();
}


int MapView::getMaxZoom(){
    return floor(maxZoom);
}

int MapView::getMinZoom(){
    return ceil(minZoom);
}
