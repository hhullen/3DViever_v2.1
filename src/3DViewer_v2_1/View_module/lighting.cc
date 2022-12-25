#include "lighting.h"
#include "ui_lighting.h"

Lighting::Lighting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Lighting)
{
    ui->setupUi(this);
}

Lighting::~Lighting()
{
    delete ui;
}
