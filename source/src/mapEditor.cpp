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
//!         Jasper Schoenmaker, 1661818
//! \date   Created: 06-04-2016
//! \date   Last Modified: 10-06-2016
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
#include "mapEditor.hpp"


MapEditor::MapEditor(QWidget *parent) :
    MapView(parent)
    {

    }

void MapEditor::removeTile(){
    editTile(QString("Unknown"));
}

void MapEditor::editTile(QString type){

    //set new boxInfo
    r2d2::BoxInfo info;
    if(type == "Navigatable"){
        info = r2d2::BoxInfo(false, true, false);
    }else if(type == "Obstacle"){
        info = r2d2::BoxInfo(true, false, false);
    }else if(type == "Unknown"){
        info = r2d2::BoxInfo(false, false, true);
    }else{
        info = r2d2::BoxInfo(true, true, false);
    }

    //store
    map->set_box_info(selectedBox, info);
    drawSingleBox(selectedBox, info);
}


