#include "PathfinderEditTab.hpp"
#include "ui_PathfinderEditTab.h"

PathfinderEditTab::PathfinderEditTab(PathfinderModule *module, QWidget *parent) :
        module{module},
        QWidget(parent),
        ui(new Ui::PathfinderEditTab) {
    ui->setupUi(this);
}

PathfinderEditTab::~PathfinderEditTab() {
    delete ui;
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

    r2d2::Coordinate start{};
    std::vector<r2d2::Coordinate> path{};
    std::cout << module->pather->get_path_to_coordinate(start, {100 * r2d2::Length::METER,
                                                                100 * r2d2::Length::METER,
                                                                0 * r2d2::Length::METER}, path) << std::endl;
    r2d2::Coordinate prevPos{start};
    for (r2d2::Coordinate &coord : path) {
        std::cout << coord << std::endl;
        module->mapEditorPointer->scene->drawLine(prevPos, coord, Qt::blue);
        prevPos = coord;
    }
}
