#include "mapView.hpp"
#include "ArrayBoxMap.hpp"
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

void mapView::increaseZoom(){
    scaleSize += zoomSpeed;
    if(scaleSize > maxZoom){
        scaleSize = maxZoom;
    }

    updateTransform();
}

void mapView::decreaseZoom(){
    scaleSize -= zoomSpeed;
    if(scaleSize < minZoom){
        scaleSize = minZoom;
    }
    updateTransform();
}

int mapView::getScale(){
    //give range 0-100%
    return scaleSize * 100 / maxZoom;
}

void mapView::resetScale(){
    scaleSize = (maxZoom / 2);
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
    drawMap();
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
    QRectF newBoxArea = scene->selectionArea().boundingRect();
    r2d2::Box box(scene->qpoint_2_box_coordinate(
                  newBoxArea.bottomLeft(), z_bottom),
                  scene->qpoint_2_box_coordinate(newBoxArea.topRight(), z_top)
                  );

    selectedBox = box;
    drawMap();
}

selectionData mapView::getSelectionData(){
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

    r2d2::Coordinate topLeft(scene->
                             qpoint_2_box_coordinate(
                                 selection.topLeft(),z_bottom));
    r2d2::Coordinate bottomRight(scene->
                                 qpoint_2_box_coordinate(
                                     selection.bottomRight(),z_top));

    selData.xtop = topLeft.get_x()/r2d2::Length::CENTIMETER;
    selData.ytop = topLeft.get_y()/r2d2::Length::CENTIMETER;
    selData.xbottom = bottomRight.get_x()/r2d2::Length::CENTIMETER;
    selData.ybottom = bottomRight.get_y()/r2d2::Length::CENTIMETER;
    selData.width = selection.width();
    selData.height = selection.height();
    return selData;
}

bool mapView::event(QEvent *event)
{
        switch(event->type()){
                case QEvent::KeyPress:{
                    QKeyEvent * ke = static_cast<QKeyEvent*>(event);
                    // std::cout << "key pressed in @ event filter in
                    //mainwindow " << ke->key() << std::endl;
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
        recenterMap();
        drawMap();
    }

void mapView::saveMapFile(std::string file){
    map->save(file);

}

bool mapView::eventFilter(QObject * object, QEvent * event){
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
       case QEvent::GraphicsSceneMouseRelease:
           {
               QGraphicsSceneMouseEvent * gsme = static_cast<QGraphicsSceneMouseEvent*>(event);
               if(dragMode() == QGraphicsView::RubberBandDrag && (gsme->button() == Qt::MouseButton::RightButton)){
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
        QPointF topLeft(mapToScene(QPoint(0,height())));
        QPointF bottemRight(mapToScene(QPoint(width(),0)));

        r2d2::Box screenToSceneCoordinate(scene->
                                          qrect_2_box_coordinate(
                                              QRectF(topLeft, bottemRight),
                                              z_bottom, z_top));

        //This renders only what is in the view after scrolling
        std::vector<std::pair<r2d2::Box, r2d2::BoxInfo>> boxesOnScreen =
                map->get_intersecting(screenToSceneCoordinate);

        for(std::pair<r2d2::Box, r2d2::BoxInfo> pair: boxesOnScreen){
            drawSingleBox(pair.first, pair.second);
        }
        scene->drawAxes();

    }

void mapView::drawSingleBox(r2d2::Box box, r2d2::BoxInfo info){
    const r2d2::Coordinate bottemLeft{
                          box.get_bottom_left().get_x() ,
                           box.get_bottom_left().get_y() ,
                            r2d2::Length::CENTIMETER * z_bottom};
    const r2d2::Coordinate topRight{
                           box.get_top_right().get_x() ,
                           box.get_top_right().get_y() ,
                           r2d2::Length::CENTIMETER * z_top};

    r2d2::Box tempbox(bottemLeft,topRight);
    scene->drawTile(tempbox, tileColors[getTileType(info)]);
}

void mapView::recenterMap(){
        centerOn(scene->box_coordinate_2_qpoint(
                     r2d2::Coordinate(
                         0*r2d2::Length::CENTIMETER,
                         0*r2d2::Length::CENTIMETER,
                         0*r2d2::Length::CENTIMETER)));
    }

void mapView::mouseReleaseEvent(QMouseEvent* event){
    drawMap();
    QGraphicsView::mouseReleaseEvent(event);
}


void mapView::emptyMap(){
    map = new r2d2::ArrayBoxMap();
}


int mapView::getMaxZoom(){
    return floor(maxZoom);
}

int mapView::getMinZoom(){
    return ceil(minZoom);
}
