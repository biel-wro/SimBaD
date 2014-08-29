#include "simulation_dialog.h"
#include "ui_simulation_dialog.h"
#include <GL/glu.h>
Simulation_Dialog::Simulation_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Simulation_Dialog)
{
    ui->setupUi(this);
}

void Simulation_Dialog::initializeGL()
{

}
void Simulation_Dialog::paintGL()
{

}
void Simulation_Dialog::resize(int w,int h)
{

}

Simulation_Dialog::~Simulation_Dialog()
{
    delete ui;
}

void Simulation_Dialog::on_buttonBox_accepted()
{

}
