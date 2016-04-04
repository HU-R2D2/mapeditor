

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->drawTile(10,10,10,10,"blocked");
    ui->graphicsView->drawTile(10,10,10,10,"blocked");
    ui->graphicsView->clear();
    ui->graphicsView->drawTile(10,100,100,1000,"unkown");
    ui->graphicsView->drawTile(900,100,100,1000,"unkown");
    ui->graphicsView->drawTile(200,10,10,10,"free");
    ui->graphicsView->setTag(100, 100, QString("Hallo! Dit is een test"));

    //hijacking scrolbar events
    ui->graphicsView->verticalScrollBar()->installEventFilter(this);
    ui->graphicsView->horizontalScrollBar()->installEventFilter(this);
    //install event filter for graphicsView
    ui->graphicsView->installEventFilter(this);
    ui->graphicsView->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete viewer;
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
}

void MainWindow::on_zoomOutButtom_clicked()
{
}

bool MainWindow::event(QEvent *event)
{
        return QWidget::event(event);

        //::cout<<"mainWindow event type"<< event->type()<<std::endl;
        if (event->type() == QEvent::MouseButtonPress){
                QPoint p = ui->graphicsView->mapFromGlobal(QCursor::pos());
                //QPoint p = ui->graphicsView->view->mapFromGlobal(QCursor::pos());
                std::cout << "mouse pos: x"<< p.x() << " y" <<p.y()<<std::endl;
                ui->xposLabel->setText(QString::number(p.x()));
                ui->yposLabel->setText(QString::number(p.y()));

                if (ui->graphicsView->mouseInMapView(p)){

                std::cout << "mouse click is in viewer"  <<std::endl;}
                else {std::cout << "mouse click is not in viewer"  <<std::endl;}
            }
        fflush(stdout);
    return QWidget::event(event);
}

void MainWindow::on_actionPan_toggled(bool activatePan)
{
    if(activatePan){
            //QEvent * harry = new QEvent(QEvent::Enter);
            //QCoreApplication::sendEvent(ui->graphicsView,harry);
            ui->graphicsView->releaseMouse();
            ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            //ui->graphicsView->send

            printf("drag hand\n");
            fflush(stdout);
        }
    else{
            ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
            printf("select cursor\n");
            fflush(stdout);
        }
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    switch(event->type()){

        case QEvent::Wheel:
            if (object == ui->graphicsView->verticalScrollBar()){//catch
                QWheelEvent* we = static_cast<QWheelEvent*>(event);
                int num = we->delta();
                std::cout << "delta: "<< num<< std::endl;
                fflush(stdout);
                return true;
                }
            else if(object == ui->graphicsView->horizontalScrollBar()){//catch horizontal scroll
                return true;}
            break;

        case QEvent::MouseMove:
            {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            if(ui->graphicsView->mouseInMapView(me->pos())){
                ui->xposLabel->setText(QString::number(me->pos().x()));
                ui->yposLabel->setText(QString::number(me->pos().y()));
            } else if(true) {ui->graphicsView->releaseMouse();
                std::cout << "leaving view" << std::endl;
                }
            }
            break;
        case QEvent::MouseButtonPress:
            {QPoint p = ui->graphicsView->mapFromGlobal(QCursor::pos());
            std::cout << "mouse press @ pos: x"<< p.x() << " y" <<p.y()<<std::endl;
            fflush(stdout);}
            break;
        case QEvent::Enter:
            {
            std::cout << "succecvol enter " << object << std::endl;
            if(! ui->actionPan->isChecked()){
                ui->graphicsView->grabMouse();}
            }
            break;
        default:
            std::cout << "mapview event filter event type " << event->type() << std::endl;
            fflush(stdout);
            break;
        }
    fflush(stdout);
    return QWidget::eventFilter(object,event);
}

//void MainWindow::on_inputX_cursorPositionChanged(int arg1, int arg2)
//{
// commented to avoid linker error "Unused parameter"
//}

//void MainWindow::on_inputY_cursorPositionChanged(int arg1, int arg2)
//{
// commented to avoid linker error "Unused parameter"
//}

//testcode
void MainWindow::on_pushButton_clicked()
{
    int x = ui->xpos->value();
    int y = ui->ypos->value();
    int w = ui->width->value();
    int h = ui->Height->value();

    QString type(ui->type->currentText());
    ui->graphicsView->drawTile(x, y, w, h, type);
}

void MainWindow::on_placeTagButton_clicked()
{
    int x = ui->xposTag->value();
    int y = ui->yposTag->value();

    QString tag(ui->tagName->text());
    ui->graphicsView->setTag(x, y, tag);
}

void MainWindow::on_clearButton_clicked()
{
    ui->graphicsView->clear();
}

void MainWindow::on_actionSave_triggered()
{

}

