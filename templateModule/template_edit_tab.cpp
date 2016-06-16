#include "template_edit_tab.hpp"
#include "ui_template_edit_tab.h"

template_edit_tab::template_edit_tab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::template_edit_tab)
    {
        ui->setupUi(this);
    }

template_edit_tab::~template_edit_tab()
    {
        delete ui;
    }
