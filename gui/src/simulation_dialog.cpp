#include "simulation_dialog.h"
#include "ui_simulation_dialog.h"

Simulation_Dialog::Simulation_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Simulation_Dialog)
{
    ui->setupUi(this);
}

Simulation_Dialog::~Simulation_Dialog()
{
    delete ui;
}
