#ifndef PATHFINDER_MODULE_HPP
#define PATHFINDER_MODULE_HPP

#include <QObject>
#include <QWidget>
#include "mapeditormodule.hpp"
#include "../../../sharedobjects/source/include/LockingSharedObject.hpp"
#include "../../../pathfinding/source/include/AStarPathFinder.hpp"

class PathfinderModule : public mapeditorModule {
public:
    PathfinderModule(module_data data, QObject *parent);

private:
    friend class PathfinderEditTab;

    r2d2::SaveLoadMap *prevMap;
    std::shared_ptr<LockingSharedObject<r2d2::ReadOnlyMap>> sharedMap;
    std::shared_ptr<r2d2::AStarPathFinder> pather;
};

#endif // PATHFINDER_MODULE_HPP
