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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //hijacking scrolbar events
    ui->graphicsView->verticalScrollBar()->installEventFilter(this);
    ui->graphicsView->horizontalScrollBar()->installEventFilter(this);

    //install event filter for graphicsView
    ui->graphicsView->installEventFilter(this);
    ui->graphicsView->setMouseTracking(true);

    Map map = Map();
    ui->graphicsView->scene->drawMap(map);

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
    dialog.exec();
}

void MainWindow::on_zoomInButton_clicked()
{
    ui->graphicsView->increaseScale(0.1f);//TODO: magic value
    ui->zoomResetButton->setText(QString::number(ui->graphicsView->getScale())+ " %");
}

void MainWindow::on_zoomOutButtom_clicked()
{
    ui->graphicsView->decreaseScale(0.1f);//TODO: magic value
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
                ui->xposLabel->setText(QString::number(gsme->scenePos().x()));
                ui->yposLabel->setText(QString::number(gsme->scenePos().y()));
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
    int x = ui->xpos->value();
    int y = ui->ypos->value();
    int w = ui->width->value();
    int h = ui->Height->value();

    QString type(ui->type->currentText());

    //viewer->drawTile(x, y, w, h, type);

    ui->graphicsView->scene->drawTile(x, y, w, h, type);

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


