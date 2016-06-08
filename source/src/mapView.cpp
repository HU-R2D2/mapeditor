#include "../include/mapView.hpp"
#include "../../map/source/include/ArrayBoxMap.hpp"
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

QMutex EventRecursion;


mapView::mapView(QWidget *parent):
    QGraphicsView(parent),
    windowWidth(1000),
    windowHeight(1000)
{  
    tileColors[MapTypes::TileType::EMPTY]=Qt::green;
    tileColors[MapTypes::TileType::BLOCKED]=Qt::red;
    tileColors[MapTypes::TileType::MIXED]=Qt::yellow;
    tileColors[MapTypes::TileType::UNKNOWN]=Qt::gray;

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    scene = new viewScene;
    QPalette p = scene->palette();

    p.setColor(QPalette::Highlight, Qt::red);


    scene->setPalette(p);
    std::cout << "new Viewer with size: " << windowWidth << " x " << windowHeight << std::endl;
    scene->setSceneRect( 0, 0, windowWidth, windowHeight);
    setScene(scene);
    //editor = new mapEditor(this);
    show();

    //set default scale
    resetScale();
    scene->installEventFilter(this);
    verticalScrollBar()->installEventFilter(this);
    horizontalScrollBar()->installEventFilter(this);
    setMouseTracking(true);

    scene->clear();
    scene->addOriginOffset(250,250);
    centerOn(scene->box_coordinate_2_qpoint(r2d2::Coordinate(
                                                0*r2d2::Length::CENTIMETER,
                                                0*r2d2::Length::CENTIMETER,
                                                0*r2d2::Length::CENTIMETER)));
}

mapView::~mapView(){
    delete scene;
}

void mapView::setSelectable(bool state){
    for(QGraphicsItem * item: items()){
        if(scene->isTile(item)){
            item->setFlag(QGraphicsItem::ItemIsSelectable, state);
        }
    }
}

void mapView::increaseScale(){
    scaleSize += zoomSpeed;
    if(scaleSize > maxScale){

        scaleSize = maxScale;
    }

    updateTransform();
    //checkSceneBorder();
}

void mapView::decreaseScale(){
    scaleSize -= zoomSpeed;
    if(scaleSize < minScale){
        scaleSize = minScale;

    }
    updateTransform();
    //checkSceneBorder();
}

int mapView::getScale(){
    float normalizer = maxScale / 1;
    // multiply with 200 to make center of the scale range 100%
    return (scaleSize / normalizer) * 200;
}

void mapView::resetScale(){
    scaleSize = (maxScale / 2);
    updateTransform();
}

void mapView::increaseRotation(int inc){
    rotation = (rotation + inc) % 360;
    updateTransform();
}

void mapView::decreaseRotation(int dec){
    if(rotation == 0){
        rotation = 360 - dec;
    }else{
        rotation -= dec;
    }
    updateTransform();
}

int mapView::getRotation(){
    return rotation;
}

void mapView::resetRotation(){
    rotation = 0;
    updateTransform();
}

void mapView::updateTransform(){
    resetTransform();
    rotate(rotation);
    scale(scaleSize, scaleSize);
}

void mapView::set_z_bottom(float value)
{
    z_bottom = value;
}

void mapView::set_z_top(float value){
    z_top = value;
}


void mapView::setZoomSpeed(qreal speed){
    zoomSpeed = speed;
}

void mapView::deselectTiles(){
    for(QGraphicsItem * tile : scene->selectedItems()){
        tile->setSelected(false);
    }
}

void mapView::updateSelection(){

    //QGraphicsItemGroup selection; //= scene->createItemGroup( scene->selectedItems());

    if(scene->selectedItems().size() > 0){
        std::cout << scene->selectionArea().boundingRect().topLeft().x() << " x " << scene->selectionArea().boundingRect().topLeft().y()  <<std::endl;
        QPointF bl = scene->selectionArea().boundingRect().bottomLeft();
        QPointF tr = scene->selectionArea().boundingRect().topRight();

        //new method stores complete box
        QPointF b = scene->itemAt(QPointF(ceil(bl.x()), ceil(bl.y())), transform())->pos();
        QPointF t = scene->itemAt(QPointF(ceil(tr.x()), ceil(tr.y())), transform())->pos();
        r2d2::Coordinate leftBottom = scene->qpoint_2_box_coordinate(QPointF(b.x(), b.y()), 0);
        r2d2::Coordinate rightTop = scene->qpoint_2_box_coordinate(QPointF(t.x(), t.y()), 1);
        r2d2::Box box(leftBottom, rightTop);

        selectedBox = box;
     }
}

bool mapView::event(QEvent *event)
{
        switch(event->type()){
                case QEvent::KeyPress:{
                    QKeyEvent * ke = static_cast<QKeyEvent*>(event);
                   // std::cout << "key pressed in @ event filter in mainwindow " << ke->key() << std::endl;
                        if(ke->key() == Qt::Key_Down){
                                int val = verticalScrollBar()->value();
                                verticalScrollBar()->setValue(val+scrollStepSize);
                            }
                        else if(ke->key() == Qt::Key_Up){
                                int val = verticalScrollBar()->value();
                                verticalScrollBar()->setValue(val-scrollStepSize);
                            }
                        else if(ke->key() == Qt::Key_Right){
                                int val = horizontalScrollBar()->value();
                                horizontalScrollBar()->setValue(val+scrollStepSize);
                            }
                        else if(ke->key() == Qt::Key_Left){
                                int val = horizontalScrollBar()->value();
                                horizontalScrollBar()->setValue(val-scrollStepSize);
                            }
                    break;}

                default:
            break;
            }
        //std::cout<<"map view event type"<< event->type()<<std::endl;
        //fflush(stdout);
    return QGraphicsView::event(event);
}

void mapView::checkSceneBorder(){
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

void mapView::loadMapFile(string file)
    {
        map = new r2d2::ArrayBoxMap;
        map->load(file);
        drawMap();
    }

void mapView::saveMapFile(std::string file){
    //map = new r2d2::BoxMap;
    map->save(file);

}

bool mapView::eventFilter(QObject * object, QEvent * event){
        //return true if you want to stop the event from going to other objects
        //return false if you you do not want to kill the event.
        //event filter order parent->child->child'sChild->etc...

        checkSceneBorder();


    switch(event->type()){

       case QEvent::GraphicsSceneDragEnter:{
         std::cout << "MOVED THE SCENE";
         break;

       }

       case QEvent::Wheel:

           if (object == verticalScrollBar()){//catch
               QWheelEvent* we = static_cast<QWheelEvent*>(event);
               int num = we->delta();
               if(num < 0){
                   decreaseScale();
               }else{
                   increaseScale();
               }
               return true;
               }
           else if(object == horizontalScrollBar()){//catch horizontal scroll
               return true;}
           break;
       case QEvent::GraphicsSceneMouseMove:
           {
           //NOTE: this will give scene pos without the offset (aka, not real map pos)
           //Example code for scene mouse pos:
           //QGraphicsSceneMouseEvent * gsme = static_cast<QGraphicsSceneMouseEvent*>(event);
           //std::cout<< "mouse pos in scene is: x" << gsme->scenePos().x() << " y" << gsme->scenePos().y() << std::endl;

           return true;
           break;
           }
        default:
            break;
    }
    return false;
}

MapTypes::TileType mapView::getTileType(r2d2::BoxInfo & tileInfo){
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

void mapView::drawMap(){
        scene->clear();
        resetScale();
        scene->addOriginOffset(250,250);
        centerOn(scene->box_coordinate_2_qpoint(r2d2::Coordinate(
                                                    0*r2d2::Length::CENTIMETER,
                                                    0*r2d2::Length::CENTIMETER,
                                                    0*r2d2::Length::CENTIMETER)));

    //TODO: Fix this so it returns the view rect in scene coords
       std::vector<std::pair<r2d2::Box, r2d2::BoxInfo>> boxesOnScreen = map->get_intersecting(
       scene->qrect_2_box_coordinate(sceneRect()));




        for(std::pair<r2d2::Box, r2d2::BoxInfo> pair: boxesOnScreen){
            const r2d2::Coordinate bottemLeft{
                                   pair.first.get_bottom_left().get_x()/ 100.0,
                                   pair.first.get_bottom_left().get_y()/ 100.0,
                                   r2d2::Length::CENTIMETER * z_bottom};
            const r2d2::Coordinate topRight{
                                   pair.first.get_top_right().get_x()/ 100.0,
                                   pair.first.get_top_right().get_y()/ 100.0,
                                   r2d2::Length::CENTIMETER * z_top};

            r2d2::Box tempbox(bottemLeft,topRight);
            scene->drawTile(tempbox, tileColors[getTileType(pair.second)]);
        }
        scene->drawAxes();

    }


