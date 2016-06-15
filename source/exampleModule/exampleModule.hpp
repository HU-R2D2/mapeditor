#ifndef EXAMPLEMODULE_HPP
#define EXAMPLEMODULE_HPP

#include <QObject>
#include <QWidget>
#include "mapeditormodule.hpp"
#include "exampleEditTab.hpp"

class exampleModule : public mapeditorModule{
public:
    exampleModule(mapEditor * sad,  QObject *parent);
    };

#endif // EXAMPLEMODULE_HPP
