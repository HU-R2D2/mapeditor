#ifndef EXAMPLEMODULE_HPP
#define EXAMPLEMODULE_HPP

#include <QObject>
#include <QWidget>
#include "mapeditormodule.hpp"

class exampleModule : public mapeditorModule{
public:
    exampleModule(mapEditor * sad);
    };

#endif // EXAMPLEMODULE_HPP
