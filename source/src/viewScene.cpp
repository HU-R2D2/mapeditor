//! \addtogroup 0008 Mapviewer/-editor
//! \brief handels Drawn items in a view.
//!
//! this class extends QGraphicsScene (the qt Scene class).
//! The viewScene handels all the visual objects in the QGraphicsView.
//! It translates the the map coordinates to scene coordinated and back.
//! This is done because the Y axis is fliped and
//! the scene cannot draw in negative coordinates.
//!
//! \file   viewScene.hpp
//! \author Koen de Guijter, 1671103
//!         Jop van Buuren, 1658718
//!         Daniel Klomp, 1661521
//!         Jasper Schoenmaker, 1661818
//! \date   Created: 30-03-2016
//! \date   Last Modified: 10-06-2016
//! \brief  Header for viewScene
//!
//! This is the header file for viewScene
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

#include "viewScene.hpp"

ViewScene::ViewScene(QObject *parent) : QGraphicsScene(parent){
    // Nothing to do
}

void ViewScene::setNewOriginOffset(int unsigned xOffset,int unsigned yOffset){
    QPoint oldOffset = originOffset;
    originOffset.setX(xOffset);
    originOffset.setY(yOffset);
    QPoint relativeOffset = originOffset - oldOffset;
    setSceneRect(0,0,width() + relativeOffset.x(), height() +
                 relativeOffset.y());
    //loop to relocate all items on the scene
    for( auto item : items() ) {
        item->setPos(item->pos()+relativeOffset);
    }
    drawAxes();
}

void ViewScene::drawAxes(){
    QPen pen(Qt::blue);
    pen.setWidth(2);
    pen.setCosmetic(true);
    delete xAxis;
    delete yAxis;
    xAxis = addLine(0,originOffset.y(),width(),originOffset.y(),pen);
    yAxis = addLine(originOffset.x(),0,originOffset.x(),height(),pen);
}

void ViewScene::addOriginOffset(unsigned int x, unsigned int y)
    {
        setNewOriginOffset(originOffset.x()+x,originOffset.y()+y);
    }

QPointF ViewScene::box_coordinate_2_qpoint(r2d2::Coordinate coordinate){
    return QPointF(
        (coordinate.get_x()/r2d2::Length::CENTIMETER) + originOffset.x(),
        ((coordinate.get_y()/r2d2::Length::CENTIMETER)*-1)+originOffset.y()
    );
}

QRectF ViewScene::box_tile_2_qrect(r2d2::Box box){
    //multiply with -1 to flip the value
    return QRectF(
        (box.get_bottom_left().get_x()/r2d2::Length::CENTIMETER)
        + originOffset.x(),
        ((box.get_bottom_left().get_y()/ r2d2::Length::CENTIMETER)*-1)
        + originOffset.y(),
        (box.get_axis_size().get_x()/ r2d2::Length::CENTIMETER),
        (box.get_axis_size().get_y()/ r2d2::Length::CENTIMETER)*-1
    );
}

r2d2::Coordinate ViewScene::qpoint_2_box_coordinate(QPointF point, double z){
    // multiplied y by -1 to flip value
    return r2d2::Coordinate(
        (point.x() - originOffset.x())* r2d2::Length::CENTIMETER,
        ((point.y() - originOffset.y()))* -1 * r2d2::Length::CENTIMETER,
        z*r2d2::Length::CENTIMETER
    );
}

r2d2::Box ViewScene::qrect_2_box_coordinate(QRectF rect,
    double min_z,
    double max_z){
        return r2d2::Box(
            qpoint_2_box_coordinate(rect.bottomLeft(),min_z),
            qpoint_2_box_coordinate(rect.topRight(),max_z));
}

void ViewScene::drawTile(r2d2::Box box,QColor color){
    QRectF tempRect = box_tile_2_qrect(box);
    QGraphicsRectItem *block = new QGraphicsRectItem;
    block->setBrush(* new QBrush(color));
    //check current draw outline state
    if(!outlined){
        block->setPen(Qt::NoPen);
    }
    block->setRect(0,0,tempRect.width(),tempRect.height());
    block->setPos(tempRect.x(),tempRect.y());
    block->setVisible(true);
    addItem(block);
}



void ViewScene::clear(){
    QList<QGraphicsItem *> list = items();
    foreach( QGraphicsItem * item, list ){
        removeItem(item);
    }
    drawAxes();
}

bool ViewScene::isTile(QGraphicsItem * item){
    if(!(item == xAxis || item == yAxis || item == tmpSelection)){
        return true;
    }else{
        return false;
    }
}

//remove only the selected items
void ViewScene::deleteSelectedItems(){
    QList<QGraphicsItem *> list = selectedItems();
    foreach( QGraphicsItem * item, list ){
        removeItem(item);
    }
}

void ViewScene::setTag(r2d2::Coordinate pos, QString value){
    QGraphicsTextItem *item = addText(value);
    item->setPos(box_coordinate_2_qpoint(pos));
}

void ViewScene::deleteSelection(){
    removeItem(tmpSelection);
}

void ViewScene::drawSelection(){
    QRectF selRect = selectionArea().boundingRect();
    std::cout << selRect.x() << " x " << selRect.y() << std::endl;

    tmpSelection->setBrush(Qt::blue);
    tmpSelection->setPen(Qt::NoPen);
    tmpSelection->setRect(selRect);
    tmpSelection->setOpacity(0.3);
    tmpSelection->setVisible(true);
    addItem(tmpSelection);
}

void ViewScene::setOutline(bool state){
    outlined = state;
}
