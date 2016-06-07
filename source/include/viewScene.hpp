//! \addtogroup 0008 Mapviewer/-editor
//! \brief handels Drawn items in a view.
//!
//! this class extends QGraphicsScene (the qt Scene class).
//! The viewScene handels all the visual objects in the QGraphicsView.
//! It translates the the map coordinates to scene coordinated and back.
//! This is done because the Y axis is fliped and the scene cannot draw in negative coordinates.
//!
//! \file   viewScene.hpp
//! \author Koen de Guijter, 1671103
//!         Jop van Buuren, 1658718
//!         Daniel Klomp, 1661521
//! \date   Created: 30-03-2016
//! \date   Last Modified: 16-05-2016
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

#ifndef VIEWSCENE_HPP
#define VIEWSCENE_HPP

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <map>
#include <iostream>
#include <QPoint>
#include <QPen>
#include "Box.hpp"
#include "Coordinate.hpp"
#include "Translation.hpp"
#include "Length.hpp"

class viewScene : public QGraphicsScene
    {
    Q_OBJECT

private:

    QPoint originOffset{0,0};
    QGraphicsLineItem * xAxis = addLine(0,0,0,0);
    QGraphicsLineItem * yAxis = addLine(0,0,0,0);

public:

    //! \fn     explicit viewScene::viewScene()
    //!
    //! \brief  constructor for viewScene, creates new viewScene
    //!
    //! \param parent QWidget to be passed to QGraphicsScene
    explicit viewScene(QObject *parent = 0);

    //! \fn     void drawAxes()
    //!
    //! \brief  draws the origin axis of map in the Scene
    //! axis should be drawn on an top layer
    //! there should be an override setSceneRect() to draw axis
    //! after that is done this function could be privite (as is should be)
    void drawAxes();

    //! \fn     void viewScene::addOriginOffset()
    //!
    //! \brief  add origin offset to the scene and replaces all objects in the scene
    //!
    //! \param x added offset in centimeters
    //! \param y added offset in centimeters
    void addOriginOffset(int unsigned x, int unsigned y);

    //! \fn     void setNewOriginOffset()
    //!
    //! \brief  resets the origin offset to given values
    //!
    //! \param xOffset the new offset in centimeters
    //! \param yOffset the new offset in centimeters
    void setNewOriginOffset(int unsigned xOffset,int unsigned yOffset);

    //! \fn     void viewScene::drawLine()
    //!
    //! \brief  Draws a line from position x1,y1 to position x2,y2 on the scene.
    //!
    //! \param x1
    //! \param y1
    //! \param x2
    //! \param y2
    //! \param color of the drawn line
    void drawLine(int x1, int y1, int x2, int y2, QRgb color);

    //! \fn     void viewScene::setTag()
    //!
    //! \brief  draws a string on the viewscene
    //!
    //! \param pos the top left corner of the test (map coordinates)
    //! \param value the string that will be shown
    void setTag(r2d2::Coordinate pos, QString value);

    //! \fn     void viewScene::deleteSelectedItems()
    //!
    //! \brief  delete selected items
    void deleteSelectedItems();

    //! \fn     void viewScene::clear()
    //!
    //! \brief  Clears all the objects in the scene.
    void clear();

    //! \fn     void viewScene::drawTile()
    //!
    //! \brief  draws a tile on map pos with the given color
    //!
    //! \param box the map coordinate where the tile will be drawn
    //! \param color
    void drawTile(r2d2::Box box, QColor color);

    //! \fn     bool viewScene::isTile()
    //!
    //! \brief  checks if the item is not an axis
    //!
    //! \param  item that will be checked.
    //!
    //! \return returns true if item is not an axis.
    bool isTile(QGraphicsItem * item);


    //! \fn     bool viewScene::box_coordinate_2_qpoint()
    //!
    //! \brief translates an map coordinate to an scene QPoint (map2scene)
    //!
    //! \param coordinate must be a map coordinate
    //!
    //! \return scene coordinate
    QPointF box_coordinate_2_qpoint(r2d2::Coordinate coordinate);

    //! \fn     bool viewScene::box_tile_2_qrect()
    //!
    //! \brief translates an map box to an scene QRect
    //!
    //! \param box must be an map box
    //!
    //! \return scene rect
    QRectF box_tile_2_qrect(r2d2::Box box);

    //! \fn     bool viewScene::qpoint_2_box_coordinate()
    //!
    //! \brief translates an QPoint to an map coordinate
    //!
    //! \param point must be an scene QPointF
    //! \param z because an adt coord is 3D and an QPoint is 2D you must declare a value for Z
    //!
    //! \return map coordinate
    r2d2::Coordinate qpoint_2_box_coordinate(QPointF point, double z = 0);

    //! \fn     bool viewScene::qrect_2_box_coordinate()
    //!
    //! \brief translates an QRect to an map Box
    //!
    //! \param rect must be an scene QRectF
    //! \param min_z because an adt box is 3D and an QRectF is 2D you must declare a value for Z
    //! \param max_z because an adt box is 3D and an QRectF is 2D you must declare a value for Z
    //!
    //! \return map box
    r2d2::Box qrect_2_box_coordinate(QRectF rect, double min_z = 0, double max_z = 1);

    };

#endif // VIEWSCENE_HPP
