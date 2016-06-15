#ifndef MAPEDITORMODULE_HPP
#define MAPEDITORMODULE_HPP

#include "mapEditor.hpp"
#include <QMenu>
#include <QString>
#include <QAction>
#include <QObject>
class mapeditorModule: public QObject{

    Q_OBJECT

protected:

    //! the name of the action to activate in the QMenu
    QString module_name;

    //edit tab
    QWidget * editTab = nullptr;
    QString edit_tab_name = "edit tab";

    //info tab
    QWidget * infoTab = nullptr;
    QString info_tab_name = "info tab";


    mapEditor * mapEditorPointer;

    QTabWidget * info_tab_bar_mainwindow;
    QTabWidget * edit_tab_bar_mainwindow;


public:

    QWidget *getEditTab() const;
    QWidget *getInfoTab() const;

    void connect_module(mapEditor * map_editor, QMenu* menu, QTabWidget * info_tab_bar, QTabWidget * edit_tab_bar);

    explicit mapeditorModule(QObject *parent);

public slots:
    void action_toggled(bool);
signals:
    void set_tab_bar(QTabWidget*,QWidget*,bool,const QString&);

    };

#endif // MAPEDITORMODULE_HPP


