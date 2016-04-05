#include "mapView.hpp"
#include <iostream>
#include <QMouseEvent>
#include <QScrollBar>
#include <QInputEvent>
#include <QEnterEvent>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QCoreApplication>
#include <QKeyEvent>

mapView::mapView(QWidget *parent):
    QGraphicsView(parent),
    windowWidth(1000),
    windowHeight(1000)
{
    /*//if no height or with given, set to view size

    if(width == 0){
      windowWidth = width();
    }
    if(height == 0){
        windowHeight = height();
    }
    */    

    scene = new viewScene;
    std::cout << "new Viewer with size: " << windowWidth << " x " << windowHeight << std::endl;
    scene->setSceneRect( 0, 0, windowWidth, windowHeight);
    setScene(scene);
    show();

    //set default scale
    resetScale();



    scene->installEventFilter(this);
}

mapView::~mapView(){
    delete scene;
}





void mapView::increaseScale(qreal inc){
    if(!(scaleSize > maxScale)){
        scaleSize += inc;
    }
    updateTransform();
}

void mapView::decreaseScale(qreal dec){
    scaleSize -= dec;
    if(scaleSize < minScale){
        scaleSize = minScale;

    }
    updateTransform();
}

int mapView::getScale(){
    return scaleSize * 200;
}

void mapView::resetScale(){
    scaleSize = 0.5f;
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

void mapView::setZoomSpeed(int speed){
    zoomSpeed = speed;
}

/*  void mapView::drawMap(RectInfo *map){
 *      for (int i = 0; i <= len(map); i++;){
 *          Box box = new Box;
 *          box = map->get_2D_Box()
 *          Coordinate left_up = map->get_left_up();
 *          Coordinate right_down = map->get_right_down();
 *          drawTile(left_up.get_x(), left_up.get_y(), right_down.get_x() - left_up.get_x(),
 *                   left_up.get_y() - right_down_y(), map->get_state );
 *          }
 *  }
 */


bool mapView::event(QEvent *event)
{
        switch(event->type()){
                case QEvent::KeyPress:{
                    QKeyEvent * ke = static_cast<QKeyEvent*>(event);
                    std::cout << "key pressed in @ event filter in mainwindow " << ke->key() << std::endl;
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
    return QObject::event(event);
}

bool mapView::eventFilter(QObject * object, QEvent * event){
        //return true if you want to stop the event from going to other objects
        //return false if you you do not want to kill the event.
        //event filter order parent->child->child'sChild->etc...

       switch(event->type()){
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
