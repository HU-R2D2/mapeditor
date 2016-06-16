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
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_set_start_clicked();

    void on_set_end_clicked();

private:
    PathfinderModule *module;

    Ui::PathfinderEditTab *ui;

    r2d2::Coordinate start{};
    r2d2::Coordinate end{};
    bool set_start = false;
    bool set_end = false;

signals:
    void set_start_label(QString);
    void set_end_label(QString);
    void set_lenght_label(QString);

};

#endif // PATHFINDER_EDIT_TAB_HPP
