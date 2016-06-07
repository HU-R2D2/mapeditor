#include "../include/viewScene.hpp"

viewScene::viewScene(QObject *parent) : QGraphicsScene(parent)
    {

    }

void viewScene::setNewOriginOffset(int unsigned xOffset,int unsigned yOffset)
    {
       QPoint oldOffset = originOffset;
       originOffset.setX(xOffset);
       originOffset.setY(yOffset);
       QPoint relativeOffset = originOffset - oldOffset;
       setSceneRect(0,0,width() + relativeOffset.x(), height() + relativeOffset.y());
       //loop to relocate all items on the scene
       for( auto item : items() ) {
            item->setPos(item->pos()+relativeOffset);
           }
       drawAxes();
    }

void viewScene::drawAxes()
    {
        QPen pen(Qt::red);
        delete xAxis;
        delete yAxis;
        xAxis = addLine(0,originOffset.y(),width(),originOffset.y(),pen);
        yAxis = addLine(originOffset.x(),0,originOffset.x(),height(),pen);
    }

void viewScene::addOriginOffset(unsigned int x, unsigned int y)
    {
        setNewOriginOffset(originOffset.x()+x,originOffset.y()+y);
    }

QPointF viewScene::box_coordinate_2_qpoint(r2d2::Coordinate coordinate){
        return QPointF((coordinate.get_x()/r2d2::Length::CENTIMETER)+originOffset.x(),
        ((coordinate.get_y()/r2d2::Length::CENTIMETER)*-1)+originOffset.y());
    }

QRectF viewScene::box_tile_2_qrect(r2d2::Box box){
        return QRectF(
                    (box.get_bottom_left().get_x()/r2d2::Length::CENTIMETER)+originOffset.x(),
                    ((box.get_bottom_left().get_y()/r2d2::Length::CENTIMETER)*-1)+originOffset.y(),
                    (box.get_axis_size().get_x()/r2d2::Length::CENTIMETER),
                    (box.get_axis_size().get_y()/r2d2::Length::CENTIMETER)*-1);
    }

r2d2::Coordinate viewScene::qpoint_2_box_coordinate(QPointF point, double z){
        return r2d2::Coordinate(
                    (point.x() - originOffset.x())*r2d2::Length::CENTIMETER,
                    ((point.y() - originOffset.y()))*-1*r2d2::Length::CENTIMETER,
                    z*r2d2::Length::CENTIMETER
                    );
    }

r2d2::Box viewScene::qrect_2_box_coordinate(QRectF rect,double min_z,double max_z){

        return r2d2::Box(qpoint_2_box_coordinate(rect.bottomLeft(),min_z),
                         qpoint_2_box_coordinate(rect.topRight(),max_z));
    }

void viewScene::drawTile(r2d2::Box box,QColor color){
    QRectF tempRect = box_tile_2_qrect(box);
    QGraphicsRectItem *block = new QGraphicsRectItem;
    block->setBrush(* new QBrush(color));
    block->setRect(0,0,tempRect.width(),tempRect.height());
    block->setPos(tempRect.x(),tempRect.y());
    addItem(block);
    }

void viewScene::clear(){
    QList<QGraphicsItem *> list = items();
    foreach( QGraphicsItem * item, list ){
        removeItem(item);
    }
    drawAxes();
}

bool viewScene::isTile(QGraphicsItem * item){
    if(!(item == xAxis || item == yAxis)){
        return true;
    }else{
        return false;
    }
}

//remove only the selected items
void viewScene::deleteSelectedItems(){
    QList<QGraphicsItem *> list = selectedItems();
    foreach( QGraphicsItem * item, list ){
        removeItem(item);
    }
}

void viewScene::setTag(r2d2::Coordinate pos, QString value){
    QGraphicsTextItem *item = addText(value);
    item->setPos(box_coordinate_2_qpoint(pos));
}
