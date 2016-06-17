#include "PathfinderEditTab.hpp"
#include "ui_PathfinderEditTab.h"
#include <QMouseEvent>
#include <QScrollBar>
#include <QInputEvent>
#include <QEnterEvent>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include "Translation.hpp"

PathfinderEditTab::PathfinderEditTab(PathfinderModule *module, QWidget *parent) :
        module{module},
        QWidget(parent),
        ui(new Ui::PathfinderEditTab) {
    ui->setupUi(this);
}

PathfinderEditTab::~PathfinderEditTab() {
        delete ui;
    }

bool PathfinderEditTab::eventFilter(QObject *object, QEvent *event)
    {
            if ( event->type() == QEvent::GraphicsSceneMouseMove)
                {
                    QGraphicsSceneMouseEvent * gsme = static_cast<
                            QGraphicsSceneMouseEvent*>(event);
                    r2d2::Coordinate mouse_pos_in_map =
                            module->mapEditorPointer->scene->
                            qpoint_2_box_coordinate(gsme->scenePos());

                    if(gsme->buttons() == Qt::MouseButton::LeftButton){
                            QString txt_x(QString::number(
                                            mouse_pos_in_map.get_x()/
                                            r2d2::Length::CENTIMETER));
                            QString txt_y(QString::number(
                                            mouse_pos_in_map.get_y()/
                                            r2d2::Length::CENTIMETER));
                        if(set_start){
                                std::cout<<"start point set" <<std::endl;
                                start=mouse_pos_in_map;
                                emit set_start_label("(" + txt_x+ "," + txt_y + ") CM");
                            }else if(set_end){
                                std::cout<<"end point set" <<std::endl;
                                end=mouse_pos_in_map;
                                emit set_end_label("(" + txt_x+ "," + txt_y + ") CM");
                            }
                    }
                }
            return false;

        }

void PathfinderEditTab::on_pushButton_clicked() {
    if (module->prevMap != module->mapEditorPointer->map) {
        module->prevMap = module->mapEditorPointer->map;
        module->sharedMap = std::shared_ptr<LockingSharedObject<r2d2::ReadOnlyMap>>{new LockingSharedObject<r2d2::ReadOnlyMap>{
                                    *module->prevMap
                            }};
        module->pather = std::shared_ptr<r2d2::AStarPathFinder>{
                new r2d2::AStarPathFinder{
                    *module->sharedMap,
                    {{}, r2d2::Translation{
                        .5 * r2d2::Length::METER,
                        .5 * r2d2::Length::METER,
                        0 * r2d2::Length::METER}}}};
    }

//    r2d2::Coordinate start{};
    r2d2::Length path_length =r2d2::Length(0*r2d2::Length::CENTIMETER);
    std::vector<r2d2::Coordinate> path{};
    std::cout << module->pather->get_path_to_coordinate(start, end, path) << std::endl;
    r2d2::Coordinate prevPos{start};
    for (r2d2::Coordinate &coord : path) {
        path_length += (prevPos-coord).get_length();
        std::cout << coord << std::endl;
        module->mapEditorPointer->scene->drawLine(prevPos, coord, Qt::blue);
        prevPos = coord;
    }
    emit set_length_label(QString::number(
                              path_length/
                              r2d2::Length::CENTIMETER) + " CM");
}

void PathfinderEditTab::on_set_start_clicked()
{
 set_start=true;
 set_end=false;
}

void PathfinderEditTab::on_set_end_clicked()
{
 set_start=false;
 set_end=true;
}
