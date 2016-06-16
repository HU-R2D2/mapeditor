//! \addtogroup 0008 Mapviewer/-editor
//! \brief main gui class
//!
//! this class extends QMainWindow.
//! in this class are all the buttons, labels and
//! other gui items declared that are in the gui.
//!
//! \file   mainwindow.hpp
//! \author jessy Visch,
//!         Jop van Buuren, 1658718
//!         Daniel Klomp, 1661521
//!         Koen de Guijter, 1671103
//!         Jasper Schoenmaker, 1661818
//! \date   Created: 30-03-2016
//! \date   Last Modified: 10-06-2016
//! \brief  Header for mainwindow
//!
//! This is the header file for mainwindow
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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "mapEditor.hpp"
#include <QDesktopServices>
#include <QFileDialog>
#include <QUrl>
#include <QString>
#include <iostream>
#include <QMouseEvent>
#include <QScrollBar>
#include <QInputEvent>
#include <QEnterEvent>
#include <QEvent>
#include <string>
#include "mapeditormodule.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! Constructor for MainWindow.
    explicit MainWindow(QWidget *parent = 0);

    //! Deconstructor for MainWindow.
    ~MainWindow();

    //! \fn setTitleState();
    //!
    //! \brief set window title with the state of the map file
    void setTitleState(bool fileLoaded = false, bool fileEdited = false,
                       bool fileSaved = false);
protected:
    //! Catches all events and returns true when a event is caught.
    bool eventFilter(QObject *object, QEvent *event);
private slots:
    //! The following methods are purely for UI purposes

    //! \fn selectionChanged()
    //!
    //! \brief receiver for selectionChanged signal of secene
    //!        updates ui data fields with data of the current selection
    void selectionChanged();

    //! \fn on_actionRoboRescue_wiki_triggered();
    //!
    //! \brief receiver for ui signal button clicked wiki
    //!        opens wiki page in browser
    //!        Opens the RoboRescue wiki page on click.
    void on_actionRoboRescue_wiki_triggered();

    //! \fn on_actionSave_as_triggered();
    //!
    //! \brief receiver for ui signal button clicked save
    //!        Saves the edited map of a location of choice.
    void on_actionSave_as_triggered();

    //! \fn on_actionLoad_triggered();
    //!
    //! \brief receiver for ui signal menu item clicked load
    //!        opens dialog for file(map) selection, to load a map
    void on_actionLoad_triggered();

    //! \fn    on_zoomInButton_clicked();
    //!
    //! \brief receiver for ui signal button clicked zoomInButton
    //!        increases the zoom and updates the displayed zoom value
    void on_zoomInButton_clicked();

    //! \fn    on_zoomInButton_clicked();
    //!
    //! \brief receiver for ui signal button clicked zoomOutButton
    //!        decreases the zoom and updates the displayed zoom value
    void on_zoomOutButtom_clicked();

    //! \fn    on_actionPan_toggled(bool);
    //!
    //! \brief receiver for ui signal button clicked dragmode
    //!        Enables the drag mode for the map viewer
    void on_actionPan_toggled(bool);

    //! \fn    on_clearButton_clicked();
    //!
    //! \brief receiver for ui signal button clicked clear view
    //!        calls scene clear
    void on_clearButton_clicked();

    //! \fn    on_actionSave_triggered();
    //!
    //! \brief receiver for ui signal button clicked save
    //!        calls mapinterface save function to save the current map
    //!        of mapView
    void on_actionSave_triggered();

    //! \fn    on_zoomResetButton_clicked();
    //!
    //! \brief receiver for ui signal button clicked zoom reset
    //!
    void on_zoomResetButton_clicked();

    //! \fn    on_rotateLeftButton_clicked();
    //!
    //! \brief receiver for ui signal button clicked rotation to left
    //!        increments rotation transform value with default value
    void on_rotateLeftButton_clicked();

    //! \fn    on_rotateRightButton_clicked();
    //!
    //! \brief receiver for ui signal button clicked rotate to right
    //!        decrements rotation transform value with default value
    void on_rotateRightButton_clicked();

    //! \fn    on_resetRotationButton_clicked();
    //!
    //! \brief receiver for ui signal button clicked reset rotation
    //!        sets rotation value to zero and updates transform
    void on_resetRotationButton_clicked();

    //! \fn    on_zoomSpeedSlider_valueChanged();
    //!
    //! \brief receiver for ui signal from zoomspeed slider
    //!        set zoomspeed value equal to the slider value
    void on_zoomSpeedSlider_valueChanged(int value);

    //! \fn    on_actionSelectMode_toggled();
    //!
    //! \brief receiver for ui signal button clicked
    void on_actionSelectMode_toggled(bool arg1);

    //! \fn    on_goNavigate_clicked();
    //!
    //! \brief receiver for ui signal button clicked goNav
    //!        centers the viewport on the given x y values
    void on_goNavigate_clicked();

    //! \fn    on_actionDebug_triggered();
    //!
    //! \brief receiver for ui signal button clicked
    //!        enable mapview debug options
    void on_actionDebug_triggered();

    //! \fn    on_Set_clicked();
    //!
    //! \brief receiver for ui signal button clicked set
    //!        calls the mapeditor edit function to add a box
    void on_Set_clicked();

    //! \fn on_placeTagButton_clicked();
    //!
    //! \brief receiver for ui signal button clicked setTag
    //!        calls the mapeditor edit function to add a tmp comment
    void on_placeTagButton_clicked();

    //! \fn on_Delete_pressed();
    //!
    //! \brief receiver for ui signal button clicked delete
    //!        calls the mapeditor delete function to remove boxes
    void on_Delete_pressed();

    //! \fn     void onActionOutlinedBoxes(bool arg1)
    //!
    //! \brief receiver for ui signal button clicked outlinedBoxes
    //!        calls view and sets teh boxoutline to the new state
    //!
    //! \param outlined state
    void on_actionOutlinedBoxes_2_toggled(bool arg1);

private:
    Ui::MainWindow *ui;

    r2d2::SaveLoadMap * map;

    std::string fileName_std = "";

    //! \fn    double_coord_2_QString()
    //!
    //! \brief creates a QT printable string displaying to double values
    //!        rounded up to 2 decimals, for ui purposes
    QString double_coord_2_QString(double x, double y);

    //! \fn updateFileData();
    //!
    //! \brief updates file and path variables of currently selected file
    void updateFileData();

    std::string file;

    std::string path;

    bool edited = false;

    std::vector<mapeditorModule*> modules{};
    QMenu * nb;
public slots:
    void set_tab_bar(QTabWidget*,QWidget*,bool,const QString&);
};

#endif // MAINWINDOW_HPP
