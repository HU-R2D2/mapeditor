#include "template_module.hpp"

template_module::template_module(QObject *parent): mapeditorModule(parent)
    {
		//set template name
		module_name = "template";

        //construct & name edit tab if needed
        editTab = new template_edit_tab();
	    QString edit_tab_name = "edit tab";

		//construct & name info tab if needed
        QWidget * infoTab = new template_info_tab();
        QString info_tab_name = "info tab";
		
    }
