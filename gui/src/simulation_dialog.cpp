#include "simulation_dialog.h"

#include "ui_simulation_dialog.h"
#include <QColorDialog>
#include "my_glwidget.h"
using namespace std;
#include<qstring.h>
#include<string.h>
#include <QtMath>
#include <QtCore>

#include <iostream>


simbad::gui::Simulation_Dialog::Simulation_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Simulation_Dialog)
{
    ui->setupUi(this);

    this->ui->widget->setParentWidjetpos(this->pos());
    this->step_for_number_of_event = 1;
    this->ui->pushButton_2->setEnabled(false);



    Timer_clock = new QTimer(this);
    connect(Timer_clock, SIGNAL(timeout()), this, SLOT(dynamics()));

}

void simbad::gui::Simulation_Dialog::setModel_of_space(Model_of_space * Model)
{
     Big_model = Model;
     this->ui->widget->Big_model = Model;

    // QString String_for_particle_types ;
    // for (int i=1;i <= Big_model->get_number_of_types(); i++){
    //     String_for_particle_types = "Particles with type # " + QString::number(i);
    //     ui->comboBox_Type_of_particle->addItem(String_for_particle_types );
    // };
}




simbad::gui::Simulation_Dialog::~Simulation_Dialog()
{
    delete ui;
}

void simbad::gui::Simulation_Dialog::on_buttonBox_accepted()
{

}

void simbad::gui::Simulation_Dialog::on_pushButton_clicked()
{
    this->ui->pushButton_2->setEnabled(true);
    this->ui->pushButton->setEnabled(false);
    Timer_clock->start(5);

    //step_for_number_of_event
    //string str = to_string(this->Big_model->get_global_clock());
    //str="Time" + str;
    //QString Qstr = QString::fromStdString(str);

    //this->ui->label_time->setText(Qstr);

}

void simbad::gui::Simulation_Dialog::on_spinBox_valueChanged(const QString &arg1)
{
    step_for_number_of_event = arg1.toFloat();
}

void simbad::gui::Simulation_Dialog::on_pushButton_2_clicked()
{
   // sim_regime=false;
    this->ui->pushButton->setEnabled(true);
    this->ui->pushButton_2->setEnabled(false);
    Timer_clock->stop();


}


void simbad::gui::Simulation_Dialog::dynamics()
{

    int Number_of_occurings_bigen = this->Big_model->Number_of_occurings;
    bool i=true;
    while (i && Number_of_occurings_bigen + step_for_number_of_event > this->Big_model->Number_of_occurings){
          i=this->Big_model->timer();
    };
    this->ui->lineTime->setText(QString::number((double)this->Big_model->get_global_clock()));
    this->ui->line_number_of_events->setText(QString::number(this->Big_model->Number_of_occurings));
    this->ui->line_number_of_events_2->setText(QString::number(this->Big_model->Number_of_events_dynamics));
    this->ui->widget->repaint();

}

void simbad::gui::Simulation_Dialog::on_horizontalSlider_valueChanged(int value)
{
    this->ui->widget->scaling_factor=pow (2.0, value / 5 - 9);
    this->ui->widget->shift_X=0.0;
    this->ui->widget->shift_Y=0.0;
}

void simbad::gui::Simulation_Dialog::on_verticalScrollBar_valueChanged(int value)
{
    this->ui->widget->shift_Y= - ((value*1.0) / 5.0 - 10)*0.1*this->ui->widget->scaling_factor;

}

void simbad::gui::Simulation_Dialog::on_horizontalScrollBar_valueChanged(int value)
{
    this->ui->widget->shift_X= - ((value*1.0) / 5.0 - 10)*0.1*this->ui->widget->scaling_factor;

}
