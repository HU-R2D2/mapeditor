#include "../include/mainwindow.hpp"
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
#include "../include/mapEditor.hpp"
#include "../../../map/source/include/BoxMap.hpp"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //hijacking scrolbar events
    ui->graphicsView->verticalScrollBar()->installEventFilter(this);
    ui->graphicsView->horizontalScrollBar()->installEventFilter(this);

    //TODO: check if we need both eventfilters (check MainWindow::eventFilter(...) )
    ui->graphicsView->scene->installEventFilter(this);
    ui->graphicsView->installEventFilter(this);

    connect(ui->graphicsView->scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

    //ui zoomspeed range setup
    //ui->zoomSpeedSlider->setMaximum(floor(ui->graphicsView->getMaxZoom()/10));
    //ui->zoomSpeedSlider->setMinimum(floor(1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRoboRescue_wiki_triggered()
{
    QString link = "https://www.roborescue.nl";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    fileName_std = fileName.toUtf8().constData();
    ui->graphicsView->saveMapFile(fileName_std);
}

void MainWindow::on_actionLoad_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QStringList fileNames;
    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
        fileName_std = fileNames.first().toUtf8().constData();
        ui->graphicsView->loadMapFile(fileName_std);
    }
}

void MainWindow::on_zoomInButton_clicked()
{
    ui->graphicsView->increaseScale();
    ui->zoomResetButton->setText(QString::number(ui->graphicsView->getScale())+ " %");
}

void MainWindow::on_zoomOutButtom_clicked()
{
    ui->graphicsView->decreaseScale();//TODO: magic value
    ui->zoomResetButton->setText(QString::number(ui->graphicsView->getScale())+ " %");
}

void MainWindow::on_zoomResetButton_clicked()
{
    ui->graphicsView->resetScale();
    ui->zoomResetButton->setText(QString::number(ui->graphicsView->getScale())+ " %");
}

void MainWindow::on_actionPan_toggled(bool activatePan)
{
    if(activatePan){
            ui->actionSelectMode->setChecked(false);
            ui->graphicsView->setSelectable(false);
            ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
        }
    else{
            ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
            ui->graphicsView->setSelectable(true);
        }
}

void MainWindow::on_actionSelectMode_toggled(bool activateSelect)
{
    if(activateSelect){
        ui->actionPan->setChecked(false);
        ui->graphicsView->setSelectable(true);
        ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        ui->graphicsView->setRubberBandSelectionMode(Qt::IntersectsItemShape);
        //ui->graphicsView->setRubberBandSelectionMode(Qt::ContainsItemShape);
        //std::cout << ui->graphicsView->scene->selectedItems().size() << std::endl;
    }
    else{
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        ui->graphicsView->setSelectable(false);
    }
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    switch(event->type()){
        case QEvent::GraphicsSceneMouseMove:
            {
                QGraphicsSceneMouseEvent * gsme = static_cast<QGraphicsSceneMouseEvent*>(event);
                r2d2::Coordinate mouse_pos_in_map=ui->graphicsView->scene->qpoint_2_box_coordinate(gsme->scenePos());
                ui->xposLabel->setText(QString::number(mouse_pos_in_map.get_x()/r2d2::Length::CENTIMETER));
                ui->yposLabel->setText(QString::number(mouse_pos_in_map.get_y()/r2d2::Length::CENTIMETER));
                return false;
                break;
            }
        case QEvent::Wheel:
            {
                ui->zoomResetButton->setText(QString::number(ui->graphicsView->getScale()) + " %");
                return false;//returns false so that evenfilter in mapview can catch it too
                break;
            }
        default:
            /* keep in code for debug purposes
            std::cout << "mapview event filter event type " << event->type() << std::endl;
            fflush(stdout);
            */
            break;
        }
    return false;
}



void MainWindow::on_Set_clicked()
{
    ui->graphicsView->updateSelection();
    ui->graphicsView->editTile(ui->type->currentText());
    ui->graphicsView->drawMap();
}

void MainWindow::on_placeTagButton_clicked()
{
    int x = ui->xposTag->value();
    int y = ui->yposTag->value();
    r2d2::Coordinate pos(x*r2d2::Length::CENTIMETER,y*r2d2::Length::CENTIMETER,0*r2d2::Length::CENTIMETER);
    QString tag(ui->tagName->text());
    ui->graphicsView->scene->setTag(pos, tag);
}

void MainWindow::on_clearButton_clicked()
{
    ui->graphicsView->scene->clear();
    //Enable code below to transform the clear button to a delete map button.
    ui->graphicsView->emptyMap();
}

void MainWindow::on_actionSave_triggered()
{
    if(fileName_std == ""){
        on_actionSave_as_triggered();
    }else {
        ui->graphicsView->saveMapFile(fileName_std);
        }
}

void MainWindow::on_rotateLeftButton_clicked()
{
    ui->graphicsView->decreaseRotation();
    ui->resetRotationButton->setText(QString::number(ui->graphicsView->getRotation()));
}

void MainWindow::on_rotateRightButton_clicked()
{
    ui->graphicsView->increaseRotation();
    ui->resetRotationButton->setText(QString::number(ui->graphicsView->getRotation()));
}

void MainWindow::on_resetRotationButton_clicked()
{
    ui->graphicsView->resetRotation();
    ui->resetRotationButton->setText(QString::number(ui->graphicsView->getRotation()));
}

void MainWindow::on_zoomSpeedSlider_valueChanged(int value)
{
    ui->graphicsView->setZoomSpeed(qreal(float(value)/1000));
}

void MainWindow::on_goNavigate_clicked()
{
    r2d2::Coordinate pos(
                ui->inputX->text().toInt()*r2d2::Length::CENTIMETER,
                ui->inputY->text().toInt()*r2d2::Length::CENTIMETER,
                0*r2d2::Length::CENTIMETER);
    ui->graphicsView->centerOn(ui->graphicsView->scene->box_coordinate_2_qpoint(pos));
    ui->graphicsView->set_z_top(ui->input_z_bot->text().toFloat());
    ui->graphicsView->set_z_bottom(ui->input_z_top->text().toFloat());

}

void MainWindow::on_actionDebug_triggered()
{
   int test = ui->graphicsView->scene->items().length();
   std::cout << "items in scene items list: " << test << std::endl;

   QRectF testrect(10,10,10,10);

   std::cout<< "testrect: " <<
   testrect.left() << " " <<
   testrect.right() << " " <<
   testrect.bottom() << " " <<
   testrect.top() << std::endl;

   r2d2::Box testbox = ui->graphicsView->scene->qrect_2_box_coordinate(testrect);

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
    std::string xstr(to_string(roundf(x * 100)/100));
    std::string ystr(to_string(roundf(y * 100)/100));
    return QString::fromStdString("( "+
        xstr.substr(0, xstr.size()-4) + ", " +
        ystr.substr(0, ystr.size()-4) + " )"
    );
}

void MainWindow::on_Delete_pressed()
{
    ui->graphicsView->updateSelection();
    ui->graphicsView->removeTile();
    ui->graphicsView->drawMap();
}

void MainWindow::selectionChanged(){
    selectionData data = ui->graphicsView->getSelectionData();
    std::cout << "Updated boxinfo" << std::endl;
    ui->bd_type->setText(data.type);
    ui->bd_topl->setText(double_coord_2_QString(data.xtop, data.ytop));
    ui->bd_botr->setText(double_coord_2_QString(data.xbottom, data.ybottom));
    ui->bd_dimension->setText(double_coord_2_QString(data.width, data.height));
}
