#include "exampleInfoTab.hpp"
#include "ui_exampleInfoTab.h"

exampleInfoTab::exampleInfoTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::exampleInfoTab)
    {
        ui->setupUi(this);
    }

exampleInfoTab::~exampleInfoTab()
    {
        delete ui;
    }
