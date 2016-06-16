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

#ifndef MAPVIEW_HPP
#define MAPVIEW_HPP

#include "MapInterface.hpp"
#include "BoxMap.hpp"
#include "ArrayBoxMap.hpp"
#include "Box.hpp"
#include "Coordinate.hpp"
#include "Translation.hpp"
#include "Length.hpp"
#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QString>
#include <iostream>
#include <string>
#include "viewScene.hpp"


namespace MapTypes{
    //! \brief  Enum class that contains the possible types a box can have
    enum class TileType {
        UNKNOWN, MIXED, BLOCKED, EMPTY
    };
}
//All the data that is used for getting a selection in the scene
struct selectionData{
    QString type;
    double xtop;
    double ytop;
    double xbottom;
    double ybottom;
    double width;
    double height;
    double ztop;
    double zbottom;
};

class MapView: public QGraphicsView {
public:
    //! \fn     explicit mapView::mapView(QWidget * parent)
    //!
    //! \brief  constructor for mapView, creates QGraphicsView and sets size
    //!
    //! \param parent QWidget required for UI
    explicit MapView(QWidget *parent);

    //! \fn     explicit mapEditor::mapEditor()
    //!
    //! \brief  constructor for mapEditor, creates new mapView
    //!
    //! \param parent QWidget to be passed to mapView
    ~MapView();

    //! \fn     bool mapEditor::mouseInMapView()
    //!
    //! \brief  check if a position (e.g. mouse) is in the GraphicsView
    //!
    //! \param  QPoint position of the mouse position
    //!
    //! \return boolean value if position in view
    bool mouseInMapView(QPoint p);

    //! \fn     selection mapEditor::filterEvent()
    //!
    //! \brief  applies the scale and rotation values
    //!         as a transform over the current scene
    selectionData getSelectionData();

    //! \fn     void mapEditor::deselectTiles()
    //!
    //! \brief  Deselects the selected tiles in the view
    void deselectTiles();

    //! \fn     void mapEditor::updateSelection()
    //!
    //! \brief  updates the selectedBox with the currently selected box in view
    void updateSelection();

    //! \fn     void mapEditor::increaseScale()
    //!
    //! \brief  increases the scale value and update the view (zooming in)
    void increaseZoom();

    //! \fn     void mapEditor::decreaseScale()
    //!
    //! \brief  decreases the scale value and update the view (zooming out)
    void decreaseZoom();

    //! \fn     void mapEditor::increaseRotation()
    //!
    //! \brief  increases rotation angle with a default angle of 90
    //!         can be changed to do a degree by degree rotation
    //!         and updates the transform
    //!
    //! \param  integer value of inc to be used to increase the rotation value
    void increaseRotation(int inc = 90);

    //! \fn     void mapEditor::decreaseRotation()
    //!
    //! \brief  decreases rotation angle with a default angle of 90
    //!         can be changed to do a degree by degree rotation.
    //!         and updates the transform
    //!
    //! \param  integer value of dec to be used to decrease the rotation value
    void decreaseRotation(int dec = 90);

    //! \fn     mapEditor::resetRotation()
    //!
    //! \brief  set the rotation angle back to zero and updates the transform
    void resetRotation();

    //! \fn     void mapEditor::resetScale()
    //!
    //! \brief  set the scaling back to half of max scale (normal zoom)
    //!         and updates the transform
    void resetScale();

    //! \fn     void mapEditor::setZoomSpeed()
    //!
    //! \brief  set the increment of zoom
    //!         this enables faster/preciser zooming
    //!         with in the GraphicsView
    //!
    //! \param  qreal speed (incrementation value)
    void setZoomSpeed(qreal speed);

    //! \fn     int mapEditor::getScale()
    //!
    //! \brief  returns scale of map
    //!
    //! \return integer value of scale
    int getScale();

    //! \fn     int mapEditor::getRotation()
    //!
    //! \brief  return rotation value
    //!
    //! \return rotatiation angle in degrees
    int getRotation();

    //! \fn     mapView::checkSceneBorder()
    //!
    //! \brief  check if view is near the border of the scene
    void checkSceneBorder();

    //! \fn     void mapEditor::set_z_bottom()
    //!
    //! \brief  sets the z top value
    //!         used for the z bottom value of boxes
    //!
    //! \param  float value is the desired z bottom value
    void set_z_bottom(float value);

    //! \fn     void mapEditor::set_z_top()
    //!
    //! \brief  sets the z top value
    //!         used for the z top value of boxes
    //!
    //! \param  float value is the desired z top value
    void set_z_top(float value);

    //! \fn     void mapEditor::saveMapFile()
    //!
    //! \brief  saves a map in a file with the name of filename
    //!
    //! \param  string of path to map for io
    void saveMapFile(std::string filename);

    //! viewscene scene on which we draw
    ViewScene * scene;

    //! map which combines TileTypes to QColors for drawing
    std::map<MapTypes::TileType, QColor> tileColors;

    //! \fn     void mapEditor::loadMapFile()
    //!
    //! \brief  loads map in to the programm
    //!         from string path
    //!
    //! \param  string of path to map for io
    void loadMapFile(std::string file);

    //! \fn      mapView::drawMap()
    //!
    //! \brief   (re)draw all the objects the map
    void drawMap();

    //! \fn    void mapView::drawSingleTile()
    //!
    //! \brief draws a box on the scene pos with the given color
    //!
    //! \param box the map coordinate where the tile will be drawn
    //! \param color
    void drawSingleBox(r2d2::Box box, r2d2::BoxInfo info);

    //! \fn      mapView::emptyMap()
    //!
    //! \brief   replace the current map with a new empty one
    void emptyMap();

    //! \fn      mapView::recenterMap()
    //!
    //! \brief   recenter the map and reset the scale
    void recenterMap();


    //! \fn     void mapView::mouseReleaseEvent()
    //!
    //! \brief  used to define what should happen when a mouse button gets
    //!         released
    //!
    //! \param event the event that occured
    void mouseReleaseEvent(QMouseEvent* event);

    //! \fn     TileType mapEditor::getTileType()
    //!
    //! \brief  returns tile type enum of tile for drawing
    //!
    //! \param  the boxinfo you want the configured tiletype for
    //!
    //! \return TileType belonging to the given boxinfo
    MapTypes::TileType getTileType(r2d2::BoxInfo & tileInfo);

    //! \fn     void mapEditor::setSelectable()
    //!
    //! \brief  makes items on screen selectable
    //!         this needs to be set true when editing
    //!         but can't be true when dragging because
    //!         qt does not allow dragging on selectable items
    //!
    //! \param  state of item selectability
    void setSelectable(bool state);

    //! \fn     void mapEditor::drawBox()
    //!
    //! \brief  reads from loaded map file the box types for the coordinateds
    //!         in the given box. calls the the draw function of the viewscene
    //!         to actually draw the boxes on screen and if enabled (default)
    //!         recenters the map on the origin
    //!
    //! \param  box to be edited
    //! \param  tileSize of one small box, default is 10
    //! \param  bool if set false does not recenter the map
    void drawBox(r2d2::Box box, int tileSize = 10, bool centeron = true);

    //! \fn     void mapEditor::getMaxZoom()
    //!
    //! \brief  get the maximum zoomlevel
    //!
    //! \return returns integer value of max zoom
    int getMaxZoom();

    //! \fn     void mapEditor::getMinZoom()
    //!
    //! \brief  get the min zoomlevel
    //!
    //! \return returns integer value of min zoom
    int getMinZoom();

private:

    //! scene dimensions
    int windowWidth;
    int windowHeight;
protected:
    //! loaded map
    r2d2::BoxMap * map = new r2d2::ArrayBoxMap;

    //! \fn     bool mapEditor::event()
    //!
    //! \brief  receives key events and translates the arrow keys to
    //!         horizontal and vertical movement in the view
    //!
    //! \param  QEvent that is received from parent
    bool event(QEvent *event);

    //! \fn     bool mapEditor::eventFilter()
    //!
    //! \brief  receives events and translates the scrollwheel movement
    //!         in qt known as the scrollbar events to zooming with
    //!         the increase and decrease zoom functions
    //!         * return true if you want to stop the event from going
    //!         to other objects
    //!         * return false if you you do not want to kill the event.
    //!         * event filter order parent->child->child'sChild->etc...
    //!
    //! \param  QEvent that is received from parent
    //! \param  QObject to check if event is from scrollbars
    bool eventFilter(QObject *object, QEvent *event);

    //! \fn     bool mapEditor::updateTransform()
    //!
    //! \brief  applies the scale and rotation values
    //!         as a transform over the current scene
    void updateTransform();

    //! Transform variables
    int rotation  = 0;
    int scrollStepSize=10;

    qreal maxZoom  = 1.0f;
    qreal minZoom  = 0.01f;
    qreal zoomSpeed = maxZoom / 100;


    //! Scalesize default is half of the scale range
    qreal scaleSize = (maxZoom / 2) - minZoom;

    //! z dimension variables
    double z_bottom = -1;
    double z_top = 1;

    selectionData selData;
    //! list of selected boxes
    r2d2::Box selectedBox;

    QGraphicsRectItem * tmpSelection;
};

#endif // MAPVIEW_HPP
