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
    module_name = "template";

    //construct & name edit tab if needed
    editTab = new PathfinderEditTab(this);
    QString edit_tab_name = "edit tab";

    //construct & name info tab if needed
    infoTab = new PathfinderInfoTab();
    QString info_tab_name = "info tab";
}
