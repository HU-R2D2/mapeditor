#include "PathfinderModule.hpp"
#include "PathfinderEditTab.hpp"
#include "PathfinderInfoTab.hpp"

PathfinderModule::PathfinderModule(module_data data, QObject *parent):
        mapeditorModule{data, parent},
        prevMap{mapEditorPointer->map},
        sharedMap{std::make_shared<LockingSharedObject<r2d2::ReadOnlyMap>>(*prevMap)},
        pather{new r2d2::AStarPathFinder{
                      *sharedMap,
                      {{}, r2d2::Translation{
                        .5 * r2d2::Length::METER,
                        .5 * r2d2::Length::METER,
                        0 * r2d2::Length::METER}}
               }} {
    //set template name
    module_name = "Pathfinder_demo";

    //construct & name edit tab if needed
    editTab = new PathfinderEditTab(this);
    edit_tab_name = "Pathfinder";

    //construct & name info tab if needed
    infoTab = new PathfinderInfoTab();
    info_tab_name = "Pathfinder";

    QObject::connect(editTab,SIGNAL(set_start_label(QString)),
                     infoTab,SLOT(set_start(QString)));
    QObject::connect(editTab,SIGNAL(set_end_label(QString)),
                     infoTab,SLOT(set_end(QString)));
    QObject::connect(editTab,SIGNAL(set_length_label(QString)),
                     infoTab,SLOT(set_length(QString)));

    mapEditorPointer->scene->installEventFilter(editTab);
}
