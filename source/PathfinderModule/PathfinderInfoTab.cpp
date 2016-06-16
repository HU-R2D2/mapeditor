#include "PathfinderInfoTab.hpp"
#include "ui_PathfinderInfoTab.h"

PathfinderInfoTab::PathfinderInfoTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathfinderInfoTab)
    {
        ui->setupUi(this);
    }

PathfinderInfoTab::~PathfinderInfoTab()
    {
        delete ui;
    }
