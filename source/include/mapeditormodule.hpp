#ifndef MAPEDITORMODULE_HPP
#define MAPEDITORMODULE_HPP

#include "mapEditor.hpp"
#include <QMenu>
#include <QString>
#include <QAction>
#include <QObject>

//! standart mapeditor data that is used for the module
struct module_data {
  MapEditor * mapeditor;
  QTabWidget * info;
  QTabWidget * edit;
};

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


    MapEditor * mapEditorPointer;

    QTabWidget * info_tab_bar_mainwindow;
    QTabWidget * edit_tab_bar_mainwindow;


public:

    QWidget *getEditTab() const;
    QWidget *getInfoTab() const;

    void connect_module(QMenu* menu);

    explicit mapeditorModule(module_data data, QObject *parent);

public slots:
    void action_toggled(bool);
signals:
    void set_tab_bar(QTabWidget*,QWidget*,bool,const QString&);

    };

#endif // MAPEDITORMODULE_HPP


