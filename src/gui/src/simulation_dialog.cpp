#include "simulation_dialog.h"

#include "my_glwidget.h"
#include "ui_simulation_dialog.h"
#include <QColorDialog>
using namespace std;
#include <QtCore>
#include <QtMath>
#include <qstring.h>
#include <string.h>

#include <iostream>

simbad::gui::Simulation_Dialog::Simulation_Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Simulation_Dialog)
{
  ui->setupUi(this);

  this->ui->widget->setParentWidjetpos(this->pos());
  this->step_for_number_of_event = 1;
  this->ui->pushButton_pause->setEnabled(false);

  Timer_clock = new QTimer(this);
  connect(Timer_clock, SIGNAL(timeout()), this, SLOT(dynamics()));
}

void simbad::gui::Simulation_Dialog::setModel_of_space(Model_of_space *Model)
{
  Big_model = Model;
  this->ui->widget->Big_model = Model;

  // QString String_for_particle_types ;
  // for (int i=1;i <= Big_model->get_number_of_types(); i++){
  //     String_for_particle_types = "Particles with type # " +
  //     QString::number(i);
  //     ui->comboBox_Type_of_particle->addItem(String_for_particle_types );
  // };
}

simbad::gui::Simulation_Dialog::~Simulation_Dialog() { delete ui; }
void simbad::gui::Simulation_Dialog::on_pushButton_start_sim_clicked()
{
  this->ui->pushButton_pause->setEnabled(true);
  this->ui->pushButton_start_sim->setEnabled(false);

  this->ui->pushButton_Save_conf->setEnabled(false);
  this->ui->spinBox_saving_step->setEnabled(false);
  this->ui->pushButton_Save_results->setEnabled(false);

  Timer_clock->start(5);
}

void simbad::gui::Simulation_Dialog::on_spinBox_valueChanged(
    const QString &arg1)
{
  step_for_number_of_event = arg1.toFloat();
}

void simbad::gui::Simulation_Dialog::on_pushButton_pause_clicked()
{
  // sim_regime=false;
  this->ui->pushButton_start_sim->setEnabled(true);
  this->ui->pushButton_pause->setEnabled(false);

  if(ui->groupBox_saving_management->isChecked())
  {
    this->ui->pushButton_Save_conf->setEnabled(true);
    this->ui->pushButton_Save_results->setEnabled(true);
    this->ui->spinBox_saving_step->setEnabled(true);
  };

  Timer_clock->stop();
}

void simbad::gui::Simulation_Dialog::dynamics()
{
  int Number_of_occurings_bigen = this->Big_model->Number_of_occurings;
  bool i = true;
  while(i &&
        Number_of_occurings_bigen + step_for_number_of_event >
            this->Big_model->Number_of_occurings)
  {
    i = this->Big_model->timer();
  };
  // this->ui->lineTime->setText(QString::number());
  // this->ui->lcdNumber_Time->value(get_global_clock());
  // this->ui->line_number_of_events->setText(QString::number(this->Big_model->Number_of_occurings));
  // this->ui->line_number_of_events_2->setText(QString::number(this->Big_model->Number_of_events_dynamics));

  this->ui->lcdNumber_Time->display(
      (double)this->Big_model->get_global_clock());
  this->ui->lcdNumber_events->display(this->Big_model->Number_of_occurings);
  this->ui->lcdNumber_attempts->display(
      this->Big_model->Number_of_events_dynamics);
  this->ui->widget->repaint();
}

void simbad::gui::Simulation_Dialog::on_horizontalSlider_valueChanged(int value)
{
  this->ui->widget->scaling_factor = pow(2.0, value / 5 - 9);
  this->ui->widget->shift_X = 0.0;
  this->ui->widget->shift_Y = 0.0;
}

void simbad::gui::Simulation_Dialog::on_verticalScrollBar_valueChanged(
    int value)
{
  this->ui->widget->shift_Y =
      -((value * 1.0) / 5.0 - 10) * 0.1 * this->ui->widget->scaling_factor;
}

void simbad::gui::Simulation_Dialog::on_horizontalScrollBar_valueChanged(
    int value)
{
  this->ui->widget->shift_X =
      -((value * 1.0) / 5.0 - 10) * 0.1 * this->ui->widget->scaling_factor;
}

void simbad::gui::Simulation_Dialog::on_pushButton_Close_clicked()
{
  this->close();
}

void simbad::gui::Simulation_Dialog::on_groupBox_saving_management_clicked(
    bool checked)
{
  if(checked && ui->pushButton_pause->isEnabled() == false)
  {
    ui->pushButton_Save_conf->setEnabled(true);
    ui->pushButton_Save_results->setEnabled(true);
    this->ui->spinBox_saving_step->setEnabled(true);
    Big_model->set_saving_step(ui->spinBox_saving_step->value());
  }
  else
  {
    ui->pushButton_Save_conf->setEnabled(false);
    ui->pushButton_Save_results->setEnabled(false);
    this->ui->spinBox_saving_step->setEnabled(false);
  };

  if(checked == false)
    Big_model->set_saving_step(0);
}

void simbad::gui::Simulation_Dialog::on_pushButton_Save_results_clicked()
{
  QFile file, conf_file;
  QString File_name = QFileDialog::getSaveFileName(
      this, tr("Save File"), "Simulation_Result", tr("Results (*.rst)"));
  file.setFileName(File_name);
  if(file.exists())
  {
    file.remove();
  }
  file.setFileName(File_name);

  conf_file.setFileName(Big_model->get_conf_full_file_name());

  if(File_name != "" && file.open(QIODevice::ReadWrite | QIODevice::Text) &&
     conf_file.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    Big_model->set_result_full_fill_name(File_name);

    QTextStream in(&file), out(&conf_file);

    do
    {
      in << out.readLine() << "\n";
    } while(out.atEnd() != true);

    QStringList File_info = Big_model->get_results_of_simulation_for_file();

    for(auto it = std::begin(File_info); it != std::end(File_info); ++it)
      in << *it;
  };
}

void simbad::gui::Simulation_Dialog::on_pushButton_Save_conf_clicked() {}
void simbad::gui::Simulation_Dialog::on_spinBox_saving_step_valueChanged(
    int arg1)
{
  Big_model->set_saving_step(arg1);
}
