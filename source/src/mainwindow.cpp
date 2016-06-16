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
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
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
#include <QGraphicsSceneMouseEvent>
#include "mapEditor.hpp"
#include "BoxMap.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //hijacking scrolbar events
    ui->graphicsView->verticalScrollBar()->installEventFilter(this);
    ui->graphicsView->horizontalScrollBar()->installEventFilter(this);

    ui->graphicsView->scene->installEventFilter(this);
    ui->graphicsView->installEventFilter(this);

    ui->graphicsView->scene->addOriginOffset(
        ui->graphicsView->width(),
        ui->graphicsView->height()
    );

    ui->graphicsView->recenterMap();

    connect(
        ui->graphicsView->scene,
        SIGNAL(selectionChanged()),
        this,
        SLOT(selectionChanged())
    );
}

MainWindow::~MainWindow(){
    delete ui;
    delete map;
}

void MainWindow::on_actionRoboRescue_wiki_triggered(){
    QString link = "https://www.roborescue.nl";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_actionSave_as_triggered(){
    QString fileName =
        QFileDialog::getSaveFileName(
            this,
            tr("Save File"),
            QString(),
            tr("map (*.map)")
        );
    fileName_std = fileName.toUtf8().constData();
    ui->graphicsView->saveMapFile(fileName_std);
    edited = false;
    setTitleState(true, false, true);
}

void MainWindow::updateFileData(){
    std::size_t cutoff = fileName_std.find_last_of("/\\");
    path = std::string(fileName_std.substr(0, cutoff));
    file = std::string(fileName_std.substr(cutoff + 1));

    if(path == ""){
        path = "-";
    }

    if(file == ""){
        file = "-";
    }

    ui->fd_path->setText(QString::fromStdString(path));
    ui->fd_name->setText(QString::fromStdString(file));
}

void MainWindow::on_actionLoad_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("map (*.map)");
    QStringList fileNames;
    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
        fileName_std = fileNames.first().toUtf8().constData();
        ui->graphicsView->loadMapFile(fileName_std);
    }
    //set filename in ui (filter out the path)
    updateFileData();

    //set windowTitle
    setTitleState(true);
}

void MainWindow::on_zoomInButton_clicked(){
    ui->graphicsView->increaseZoom();
    ui->zoomResetButton->setText(
        QString::number(ui->graphicsView->getScale())+ " %");
}

void MainWindow::on_zoomOutButtom_clicked(){
    ui->graphicsView->decreaseZoom();
    ui->zoomResetButton->setText(
        QString::number(ui->graphicsView->getScale())+ " %"
    );
}

void MainWindow::on_zoomResetButton_clicked(){
    ui->graphicsView->resetScale();
    ui->zoomResetButton->setText(
        QString::number(ui->graphicsView->getScale())+ " %"
    );
}

void MainWindow::on_actionPan_toggled(bool activatePan){
    if(activatePan){
        ui->actionSelectMode->setChecked(false);
        ui->graphicsView->setSelectable(false);
        ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    }else{
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        ui->graphicsView->setSelectable(true);
    }
}

void MainWindow::on_actionSelectMode_toggled(bool activateSelect){
    if(activateSelect){
        ui->actionPan->setChecked(false);
        ui->graphicsView->setSelectable(true);
        ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        ui->graphicsView->setRubberBandSelectionMode(
                    Qt::IntersectsItemBoundingRect);
    }
    else{
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        ui->graphicsView->setSelectable(false);
    }
}



bool MainWindow::eventFilter(QObject *, QEvent *event){
    switch(event->type()){
        case QEvent::GraphicsSceneMouseMove:
            {
                QGraphicsSceneMouseEvent * gsme = static_cast<
                        QGraphicsSceneMouseEvent*>(event);
                r2d2::Coordinate mouse_pos_in_map =
                        ui->graphicsView->scene->
                        qpoint_2_box_coordinate(gsme->scenePos());

                ui->xposLabel->setText(
                            QString::number(
                                mouse_pos_in_map.get_x()/
                                r2d2::Length::CENTIMETER));

                ui->yposLabel->setText(
                            QString::number(
                                mouse_pos_in_map.get_y()/
                                r2d2::Length::CENTIMETER));

                // selectionchanged is only fired when over selectable items,
                // to keep updating selection data whe need to manualy trigger
                // it when outside the drawn items (unkown area).
                if((gsme->buttons() == Qt::MouseButton::RightButton) ){
                    if(ui->graphicsView->dragMode() ==
                            QGraphicsView::RubberBandDrag){
                        selectionChanged();
                    }
                }
                return false;
                break;
            }

        case QEvent::Wheel:
            {
                ui->zoomResetButton->setText(
                            QString::number(
                                ui->graphicsView->getScale()) + " %");
                return false;
                break;
            }
        default:
            // cout on event-type() can be added here to display extra events
            break;
        }
    return false;
}

void MainWindow::on_Set_clicked(){
    ui->graphicsView->updateSelection();
    ui->graphicsView->editTile(ui->type->currentText());
    if(!edited){
        setTitleState(true, true);
        edited = true;
    }
}

void MainWindow::on_placeTagButton_clicked(){
    int x = ui->xposTag->value();
    int y = ui->yposTag->value();
    r2d2::Coordinate pos(
                x*r2d2::Length::CENTIMETER,
                y*r2d2::Length::CENTIMETER,
                0*r2d2::Length::CENTIMETER);
    QString tag(ui->tagName->text());
    ui->graphicsView->scene->setTag(pos, tag);
}

void MainWindow::on_clearButton_clicked(){
    ui->graphicsView->scene->clear();
    ui->graphicsView->emptyMap();
    ui->fd_name->setText(QString::fromStdString("-"));

    //resets title state
    file = "-";
    path = "-";
    setTitleState();
}

void MainWindow::on_actionSave_triggered(){
    if(fileName_std == ""){
        on_actionSave_as_triggered();
    }else {
        ui->graphicsView->saveMapFile(fileName_std);
    }
    setTitleState(true, false, true);
    edited = false;
}

void MainWindow::setTitleState(
        bool fileLoaded,
        bool fileEdited,
        bool fileSaved
    ){
    std::string name = "R2D2 Map Editor";
    updateFileData();
    if(fileLoaded){
        name += " [ " + file + " ] ";
    }
    if(fileEdited){
        name += "- edited";
    }else if(fileSaved){
        name += "- saved";
    }
    setWindowTitle(QString::fromStdString(name));
}

void MainWindow::on_rotateLeftButton_clicked(){
    ui->graphicsView->decreaseRotation();
    ui->resetRotationButton->setText(
        QString::number(ui->graphicsView->getRotation())
    );
}

void MainWindow::on_rotateRightButton_clicked(){
    ui->graphicsView->increaseRotation();
    ui->resetRotationButton->setText(
        QString::number(ui->graphicsView->getRotation())
    );
}

void MainWindow::on_resetRotationButton_clicked(){
    ui->graphicsView->resetRotation();
    ui->resetRotationButton->setText(
        QString::number(ui->graphicsView->getRotation())
    );
}

void MainWindow::on_zoomSpeedSlider_valueChanged(int value){
    ui->graphicsView->setZoomSpeed(qreal(float(value)/1000));
}

void MainWindow::on_goNavigate_clicked(){
    r2d2::Coordinate pos(
        ui->inputX->text().toInt()*r2d2::Length::CENTIMETER,
        ui->inputY->text().toInt()*r2d2::Length::CENTIMETER,
        0*r2d2::Length::CENTIMETER
    );

    ui->graphicsView->centerOn(
                ui->graphicsView->scene->box_coordinate_2_qpoint(pos));
    ui->graphicsView->set_z_top(ui->input_z_bot->text().toFloat());
    ui->graphicsView->set_z_bottom(ui->input_z_top->text().toFloat());

    ui->graphicsView->drawMap();
}

void MainWindow::on_actionDebug_triggered(){
   int test = ui->graphicsView->scene->items().length();
   std::cout << "items in scene items list: " << test << std::endl;

   QRectF testrect(10,10,10,10);

   std::cout<< "testrect: " <<
   testrect.left() << " " <<
   testrect.right() << " " <<
   testrect.bottom() << " " <<
   testrect.top() << std::endl;

   r2d2::Box testbox = ui->graphicsView->scene->
           qrect_2_box_coordinate(testrect);

   std::cout << "testbox: " <<
   testbox.get_bottom_left().get_x() << " " <<
   testbox.get_top_right().get_x() << " " <<
   testbox.get_bottom_left().get_y() << " " <<
   testbox.get_top_right().get_y() << std::endl;

   QRectF testrect2 = ui->graphicsView->scene->box_tile_2_qrect(testbox);

   std::cout<< "testrect2: " <<
   testrect2.left() << " " <<
   testrect2.right() << " " <<
   testrect2.bottom() << " " <<
   testrect2.top() << std::endl;

   testbox = ui->graphicsView->scene->qrect_2_box_coordinate(testrect);

   std::cout << "testbox2: " <<
   testbox.get_bottom_left().get_x() << " " <<
   testbox.get_top_right().get_x() << " " <<
   testbox.get_bottom_left().get_y() << " " <<
   testbox.get_top_right().get_y() << std::endl;
}

QString MainWindow::double_coord_2_QString(double x, double y){
    std::string xstr(std::to_string(roundf(x * 100)/100));
    std::string ystr(std::to_string(roundf(y * 100)/100));
    return QString::fromStdString("( "+
        xstr.substr(0, xstr.size()-4) + ", " +
        ystr.substr(0, ystr.size()-4) + " )"
    );
}

void MainWindow::on_Delete_pressed(){
    ui->graphicsView->updateSelection();
    ui->graphicsView->removeTile();
    ui->graphicsView->drawMap();
    if(!edited){
        setTitleState(true, true);
        edited = true;
    }
}

void MainWindow::selectionChanged(){
    selectionData data = ui->graphicsView->getSelectionData();
    std::cout << "Updated boxinfo" << std::endl;
    ui->bd_type->setText(data.type);
    ui->bd_topl->setText(double_coord_2_QString(data.xtop, data.ytop));
    ui->bd_botr->setText(double_coord_2_QString(data.xbottom, data.ybottom));
    ui->bd_dimension->setText(double_coord_2_QString(data.width, data.height));
    ui->bd_zcoord->setText(double_coord_2_QString(data.ztop, data.zbottom));
}

void MainWindow::on_actionOutlinedBoxes_2_toggled(bool arg1){
    ui->graphicsView->scene->setOutline(arg1);
    ui->graphicsView->drawMap();
}
