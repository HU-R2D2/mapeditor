#include "exampleEditTab.hpp"
#include "ui_exampleEditTab.h"

exampleEditTab::exampleEditTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::exampleEditTab)
    {
        ui->setupUi(this);
    }

exampleEditTab::~exampleEditTab()
    {
        delete ui;
    }
