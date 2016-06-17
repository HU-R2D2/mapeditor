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

void PathfinderInfoTab::set_start(QString txt)
    {
        ui->startpos_label->setText(txt);
    }

void PathfinderInfoTab::set_end(QString txt)
    {
        ui->endpos_label->setText(txt);
    }

void PathfinderInfoTab::set_length(QString txt)
    {
        ui->pathlengh_label->setText(txt);
    }
