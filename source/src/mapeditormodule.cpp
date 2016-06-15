#include "mapeditorModule.hpp"

QWidget *mapeditorModule::getInfoTab() const
{
        return infoTab;
    }

void mapeditorModule::connect_module(QMenu *menu,QTabWidget * info_tab_bar, QTabWidget * edit_tab_bar)
    {
        info_tab_bar_mainwindow = info_tab_bar;
        edit_tab_bar_mainwindow = edit_tab_bar;
        QAction * action = menu->addAction(module_name);
        action->setCheckable(true);
        QObject::connect(
                    action,SIGNAL(toggled(bool)),
                    this,SLOT(action_toggled(bool))
                    );
    }

mapeditorModule::mapeditorModule(mapEditor *, QObject *parent): QObject(parent)
    {

    }

void mapeditorModule::action_toggled(bool show_tabs)
    {
        if(infoTab!=nullptr){
                emit this->set_tab_bar(info_tab_bar_mainwindow,infoTab,show_tabs);
            }
        if(editTab!=nullptr){
                emit this->set_tab_bar(edit_tab_bar_mainwindow,editTab,show_tabs);
            }
    }

QWidget *mapeditorModule::getEditTab() const
    {
        return editTab;
    }
