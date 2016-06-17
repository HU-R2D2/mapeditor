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
public slots:
    void set_start(QString);
    void set_end(QString);
    void set_length(QString);
};

#endif // PATHFINDER_INFO_TAB_HPP
