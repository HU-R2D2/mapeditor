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
#include "..\include\mapEditor.hpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //hijacking scrolbar events
    ui->graphicsView->verticalScrollBar()->installEventFilter(this);
    ui->graphicsView->horizontalScrollBar()->installEventFilter(this);

    //install event filter for graphicsView
    //ui->graphicsView->installEventFilter(this);


    //Map map = Map();
    //ui->graphicsView->scene->drawMap(map);

    //TODO: check if we need both eventfilters (check MainWindow::eventFilter(...) )
    ui->graphicsView->scene->installEventFilter(this);
    ui->graphicsView->installEventFilter(this);
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
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.exec();
}

void MainWindow::on_actionLoad_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QStringList fileNames;
    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
        ui->graphicsView->loadMapFile(fileNames.first().toUtf8().constData());
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
            ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
        }
    else{
            ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        }
}

void MainWindow::on_actionSelectMode_toggled(bool activateSelect)
{

        if(activateSelect){
                ui->actionPan->setChecked(false);
                ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
            }
        else{
                ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
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
                //std::cout << ui->graphicsView->scene->
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
            std::cout << "mapview event filter event type " << event->type() << std::endl;
            break;
        }
    fflush(stdout);
    //TODO: i have no clue what this return method does(for all event related methods), should be figured out....
    return false;
}



//testcode
void MainWindow::on_pushButton_clicked()
{
        std::cout << "this button has no use...." << std::endl;
//    int x = ui->xpos->value();
//    int y = ui->ypos->value();
//    int w = ui->width->value();
//    int h = ui->Height->value();

//    QString type(ui->type->currentText());

    //This is not the propper function....
    // also, the program never draws a new tile.... (only updates tiles)
    //ui->graphicsView->scene->drawTile(x, y, w, h, type);

}

void MainWindow::on_placeTagButton_clicked()
{
    int x = ui->xposTag->value();
    int y = ui->yposTag->value();

    QString tag(ui->tagName->text());
    ui->graphicsView->scene->setTag(x, y, tag);
}

void MainWindow::on_clearButton_clicked()
{
    ui->graphicsView->scene->clear();
}

void MainWindow::on_actionSave_triggered()
{

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
    ui->graphicsView->centerOn(ui->inputX->text().toInt()+ui->graphicsView->scene->getOriginOffset().x(),ui->inputY->text().toInt()+ui->graphicsView->scene->getOriginOffset().y());
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
