//! \addtogroup 0008 Mapviewer/-editor
//! \brief Functions for editing a loaded map
//!
//! this class extends mapView and implements functions to edit map boxes
//! editing map boxes is done by getting the selected area from the viewScene
//! and using the converted cordinates of these boxes to write to the map
//! using the mapinterface.
//!
//! \file   mapEditor.hpp
//! \author Jop van Buuren, 1658718
//!         Koen de Guijter, 1671103
//! \date   Created: 06-04-2016
//! \date   Last Modified: 08-05-2016
//! \brief  Header for mapEditor
//!
//! This is the header file for MapEditor
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
#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include "mapView.hpp"
#include "../../../map/source/include/MapInterface.hpp"

//! @brief      The mapEditor class is used to write to maps
class mapEditor : public mapView
{
public:
    //! \fn     explicit mapEditor::mapEditor()
    //!
    //! \brief  constructor for mapEditor, creates new mapView
    //!
    //! \param parent QWidget to be passed to mapView
    explicit mapEditor(QWidget *parent = 0);

    //! \fn     void mapEditor::removeTile()
    //!
    //! \brief  sets boxinfo to unkown
    void removeTile();

    //! \fn     void mapEditor::editTile(QString type)
    //!
    //! \brief  goes through the selected boxes and sets their boxinfo
    //!
    //! \param  type is the Qstring name of the boxinfo type
    void editTile(QString type);

    //! \fn     void mapEditor::displayEdit()
    //!
    //! \brief  deletes old boxes from the view and replaces them with the new box info
    //!
    //! \param  type is the Qstring name of the boxinfo type
    void displayEdit();

signals:

public slots:

private:
    //! Temporary storage for boxes
    std::vector<std::pair<r2d2::Box, r2d2::BoxInfo> > saveBuffer;
    };

#endif // MAPEDITOR_HPP
