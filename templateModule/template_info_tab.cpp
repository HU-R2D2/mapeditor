#include "template_info_tab.hpp"
#include "ui_template_info_tab.h"

template_info_tab::template_info_tab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::template_info_tab)
    {
        ui->setupUi(this);
    }

template_info_tab::~template_info_tab()
    {
        delete ui;
    }
