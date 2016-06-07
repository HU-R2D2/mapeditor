//! \addtogroup 0008 Mapviewer/-editor
//! \brief main gui class
//!
//! this class extends QMainWindow.
//! in this class are all the buttons, labels and other gui items declared that are in the gui.
//!
//! \file   mainwindow.hpp
//! \author jessy Visch,
//!         Jop van Buuren, 1658718
//!         Daniel Klomp, 1661521
//!         Koen de Guijter, 1671103
//! \date   Created: 30-03-2016
//! \date   Last Modified: 12-05-2016
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

protected:
    //! Catches all events and returns true when a event is caught.
    bool eventFilter(QObject *object, QEvent *event);
private slots:

    //! Opens the RoboRescue wiki page on click.
    void on_actionRoboRescue_wiki_triggered();

    //! Saves the edited map of a location of choice.
    void on_actionSave_as_triggered();

    void on_actionLoad_triggered();

    void on_zoomInButton_clicked();

    void on_zoomOutButtom_clicked();

    //! Enables the drag mode for the map viewer
    void on_actionPan_toggled(bool);

    void on_clearButton_clicked();

    void on_actionSave_triggered();

    void on_zoomResetButton_clicked();

    void on_rotateLeftButton_clicked();

    void on_rotateRightButton_clicked();

    void on_resetRotationButton_clicked();

    void on_zoomSpeedSlider_valueChanged(int value);

    void on_actionSelectMode_toggled(bool arg1);

    void on_goNavigate_clicked();

    void on_actionDebug_triggered();

    void on_Set_clicked();

    void on_placeTagButton_clicked();

    void on_Delete_pressed();


private:
    Ui::MainWindow *ui;

    r2d2::SaveLoadMap * map;

    std::string fileName_std = "";
};

#endif // MAINWINDOW_HPP
