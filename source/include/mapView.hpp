//! \addtogroup 0008 Mapviewer/-editor
//! \brief Functions for Viewing and navigating a loaded map
//!
//! this class extends Graphicsview (the qt ui viewer class).
//! it implemts a set of functions zoom, rotate and move trough a map.
//! it also enables the selection of boxes and forms a super class for mapeditor.
//! this class initiliazis the viewscene (the scene that is displayed).
//!
//! \file   mapEditor.hpp
//! \author Jop van Buuren, 1658718
//!         Daniel Klomp, 1661521
//!         Koen de Guijter, 1671103
//! \date   Created: 30-03-2016
//! \date   Last Modified: 12-05-2016
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
#include "../../../map/source/include/MapInterface.hpp"
#include "../../../map/source/include/BoxMap.hpp"
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

class mapView: public QGraphicsView
{
public:
    //! \fn     explicit mapView::mapView(QWidget * parent)
    //!
    //! \brief  constructor for mapView, creates new QGraphicsView and sets dimensions
    //!
    //! \param parent QWidget required for UI
    explicit mapView(QWidget *parent);

    //! \fn     explicit mapEditor::mapEditor()
    //!
    //! \brief  constructor for mapEditor, creates new mapView
    //!
    //! \param parent QWidget to be passed to mapView
    ~mapView();   

    //! \fn     bool mapEditor::mouseInMapView()
    //!
    //! \brief  check if a position (e.g. mouse) is in the GraphicsView
    //!
    //! \param  QPoint position of the mouse position
    //!
    //! \return boolean value if position in view
    bool mouseInMapView(QPoint p);

    //! \fn     void mapEditor::deselectTiles()
    //!
    //! \brief  Deselects the selected tiles in the view
    void deselectTiles();

    //! \fn     void mapEditor::updateSelection()
    //!
    //! \brief  updates the list of selected boxes with the currently selected boxes in view
    void updateSelection();

    //! \fn     void mapEditor::increaseScale()
    //!
    //! \brief  increases the scale of the map to increase size of boxes (zooming in)
    void increaseScale();

    //! \fn     void mapEditor::decreaseScale()
    //!
    //! \brief  decreases the scale of the map to decrease size of boxes (zooming out)
    void decreaseScale();

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
    void saveMapFile(std::string filename);

    //! viewscene scene on which we draw
    viewScene * scene;

    //! map which combines TileTypes to QColors for drawing
    std::map<MapTypes::TileType, QColor> tileColors;

    //! \fn     void mapEditor::loadMapFile()
    //!
    //! \brief  loads map in to the programm
    //!         from string path
    //!
    //! \param  string of path to map for io
    void loadMapFile(string file);

    //! \fn      mapView::drawMap()
    //!
    //! \brief   focus viewport on the origin and (re)draw the map
    void drawMap();

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
    //! \brief  reads from loaded map file the box types for the coordinateds in the given box
    //!         calls the the draw function of the viewscene to actually draw the boxes on screen
    //!         and if enabled (default) recenters the map on the origin
    //!
    //! \param  box to be edited
    //! \param  tileSize of one small box, default is 10
    //! \param  bool if set false does not recenter the map
    void drawBox(r2d2::Box box, int tileSize = 10, bool centeron = true);

private:

    //! scene dimensions
    int windowWidth;
    int windowHeight;

protected:
    //! loaded map
    r2d2::SaveLoadMap * map;

    //! \fn     bool mapEditor::event()
    //!
    //! \brief  receives key events and translates the arrow keys to
    //!         horizontal and vertical movement in the view
    //!
    //! \param  QEvent that is received from parent
    bool event(QEvent *event);

    //! \fn     bool mapEditor::filterEvent()
    //!
    //! \brief  receives events and translates the scrollwheel movement
    //!         in qt known as the scrollbar events to zooming with
    //!         the increase and decrease zoom functions
    //!
    //! \param  QEvent that is received from parent
    //! \param  QObject to check if event is from scrollbars
    bool eventFilter(QObject *object, QEvent *event);

    //! \fn     bool mapEditor::filterEvent()
    //!
    //! \brief  applies the scale and rotation values
    //!         as a transform over the current scene
    void updateTransform();

    //! Transform variables
    int rotation  = 0;
    int scrollStepSize=10;
    qreal zoomSpeed = 0.05f;
    qreal maxScale  = 5.0f;
    qreal minScale  = 0.1f;

    //! Scalesize default is half of the scale range
    qreal scaleSize = (maxScale / 2) - minScale;

    //! z dimension variables
    int z_bottom = 0;
    int z_top = 1;

    //! list of selected boxes
    QList<r2d2::Box> selectedBoxes;
    };

#endif // MAPVIEW_HPP
