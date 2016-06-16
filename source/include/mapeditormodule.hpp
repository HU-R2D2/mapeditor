//! \addtogroup 0041 - Modulair Qt Widgets for mapViewer
//! \brief framework to make an module and add it to the mapeditor with minimal footprint.
//!
//! Base class of an module for mapeditor
//!
//! \file   mapeditormodule.hpp
//! \author koen de gruijter 1671103
//! \date   Created: 14-06-16
//! \date   Last Modified: <16-06-16>
//! \brief  Base class of an module for mapeditor
//!
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
#ifndef MAPEDITORMODULE_HPP
#define MAPEDITORMODULE_HPP

#include "mapEditor.hpp"
#include <QMenu>
#include <QString>
#include <QAction>
#include <QObject>

//! mapeditor data that is used for the module all moduels
struct module_data {
  MapEditor * mapeditor;
  QTabWidget * info;
  QTabWidget * edit;
  ~module_data(){}
};

class mapeditorModule: public QObject{

    Q_OBJECT

protected:

    //! the name of the action to activate in the QMenu
    QString module_name;

    //! edit tab pointer
    QWidget * editTab = nullptr;

    //! name of edit tab
    QString edit_tab_name = "edit tab";

    //! info tab pointer
    QWidget * infoTab = nullptr;

    //! name of info tab
    QString info_tab_name = "info tab";

    //! pointer to the mapEditor
    MapEditor * mapEditorPointer;

    //! pointer to the info tab what will show in the mapeditor
    //! if pointer != nullptr
    QTabWidget * info_tab_bar_mainwindow;

    //! pointer to the edit tab what will show in the mapeditor
    //! if pointer != nullptr
    QTabWidget * edit_tab_bar_mainwindow;

    //! status of module, true if enabled.
    bool active= false;


public:

    //! adds and connects the action button to be able to show and hide the tabs.
    //! \param menu the QMenu where the action needs to be added.
    void connect_module(QMenu* menu);

    //! constructor of an module.
    //! \data an struct of pointers of general purpuse objects for a module.
    explicit mapeditorModule(module_data data, QObject *parent);

public slots:
    void action_toggled(bool);
signals:
    void set_tab_bar(QTabWidget*,QWidget*,bool,const QString&);

    };

#endif // MAPEDITORMODULE_HPP


