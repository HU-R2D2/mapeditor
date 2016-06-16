#ifndef PATHFINDER_EDIT_TAB_HPP
#define PATHFINDER_EDIT_TAB_HPP

#include <QWidget>
#include "PathfinderModule.hpp"

namespace Ui {
    class PathfinderEditTab;
}

class PathfinderEditTab : public QWidget {
    Q_OBJECT
public:
    explicit PathfinderEditTab(PathfinderModule *module, QWidget *parent = 0);
    ~PathfinderEditTab();

private slots:
    void on_pushButton_clicked();

private:
    PathfinderModule *module;

    Ui::PathfinderEditTab *ui;
};

#endif // PATHFINDER_EDIT_TAB_HPP
