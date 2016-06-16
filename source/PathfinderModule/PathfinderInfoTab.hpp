#ifndef PATHFINDER_INFO_TAB_HPP
#define PATHFINDER_INFO_TAB_HPP

#include <QWidget>

namespace Ui {
    class PathfinderInfoTab;
}

class PathfinderInfoTab : public QWidget {
    Q_OBJECT
public:
    explicit PathfinderInfoTab(QWidget *parent = 0);
    ~PathfinderInfoTab();

private:
    Ui::PathfinderInfoTab *ui;
};

#endif // PATHFINDER_INFO_TAB_HPP
