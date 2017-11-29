#include "mainwindow.h"
#include "iostream"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore>
#include <QtGui>
#include <dialog_for_init_model.h>
#include <dialog_open_model.h>
#include <model_of_space.h>
#include <random>
#include <simulation_dialog.h>
using namespace std;

simbad::gui::MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  this->Big_model = new Model_of_space;

  ui->setupUi(this);
}

simbad::gui::MainWindow::~MainWindow()
{
  delete this->Big_model;

  delete ui;
}

void simbad::gui::MainWindow::on_actionDownload_Evolutiion_triggered()
{
  QFile file;
  QString File_name = QFileDialog::getOpenFileName(
      this, tr("Open file"), "/home", tr("Model (*.sim)"));
  file.setFileName(File_name);
  if(File_name != "" && file.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    clean_Big_model();

    this->menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(true);

    this->menuBar()->actions()[1]->menu()->actions()[0]->setEnabled(true);
    this->menuBar()->actions()[1]->menu()->actions()[1]->setEnabled(true);

    this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(false);

    QTextStream input(&file);

    QStringList Input_Info;

    do
    {
      Input_Info.push_back(input.readLine());
    } while(input.atEnd() != true);

    Big_model->set_model_setting_for_model_from_file(Input_Info);
    Big_model->set_full_file_name(file.fileName());

    this->Big_model->ModelPoints.initialisation_of_model(
        Big_model->get_number_of_types());
  };
}

void simbad::gui::MainWindow::on_actionNew_triggered()
{
  this->Big_model->initiate_before_simulation();

  Simulation_Dialog Sim_dialog_for_dyn_conf;
  Sim_dialog_for_dyn_conf.setModel_of_space(this->Big_model);
  // Sim_dialog_for_new_conf.RandomSeed=this->RandomSeed;
  Sim_dialog_for_dyn_conf.setModal(true);
  Sim_dialog_for_dyn_conf.exec();

  this->clean_Big_model();
  this->menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(false);

  this->menuBar()->actions()[1]->menu()->actions()[0]->setEnabled(false);

  this->menuBar()->actions()[1]->menu()->actions()[1]->setEnabled(false);

  this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(false);
}

void simbad::gui::MainWindow::on_actionNew_Evolution_triggered()
{
  clean_Big_model();
  Big_model->set_name_of_model("New_model");
  Dialog_open_model my_dialog_for_open_model(Big_model, true);

  // save refferens to mainForm for Big_model "main model"

  my_dialog_for_open_model.setModal(true);

  my_dialog_for_open_model.exec();
  if(this->Big_model->get_full_file_name() != "")
  {
    this->menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(true);

    this->menuBar()->actions()[1]->menu()->actions()[0]->setEnabled(true);
    this->menuBar()->actions()[1]->menu()->actions()[1]->setEnabled(true);

    this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(false);

    this->Big_model->ModelPoints.initialisation_of_model(
        Big_model->get_number_of_types());
  };
}

void simbad::gui::MainWindow::clean_init_conf_and_download_model_settings()
{
  QString File_Name_for_big_model;
  File_Name_for_big_model = this->Big_model->get_full_file_name();

  this->clean_Big_model();
  this->Big_model->set_full_file_name(File_Name_for_big_model);
  QFile File_for_Big_model;

  File_for_Big_model.setFileName(File_Name_for_big_model);

  if(!File_for_Big_model.open(QIODevice::ReadWrite | QIODevice::Text))
    return;

  QTextStream input_Big_model(&File_for_Big_model);

  QStringList File_big_model_info;

  do
  {
    File_big_model_info.push_back(input_Big_model.readLine());
  } while(input_Big_model.atEnd() != true);

  Big_model->set_model_setting_for_model_from_file(File_big_model_info);
  File_for_Big_model.close();

  this->Big_model->ModelPoints.initialisation_of_model(
      Big_model->get_number_of_types());
}

void simbad::gui::MainWindow::on_actionCreat_triggered()
{
  clean_init_conf_and_download_model_settings();

  Dialog_for_init_model Sim_dialog_for_new_conf(true);
  Sim_dialog_for_new_conf.setModel_of_space(this->Big_model);

  Sim_dialog_for_new_conf.setModal(true);
  Sim_dialog_for_new_conf.exec();

  if(this->Big_model->get_conf_full_file_name() != "")
    this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(true);
}

void simbad::gui::MainWindow::on_actionChange_project_triggered()
{
  // ///////////////////////////////////////////////
  QString File_Name;
  File_Name = this->Big_model->get_full_file_name();

  this->clean_Big_model();
  this->Big_model->set_full_file_name(File_Name);
  QFile file;

  file.setFileName(File_Name);
  if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    return;

  QTextStream input(&file);

  QStringList Input_Info;

  do
  {
    Input_Info.push_back(input.readLine());
  } while(input.atEnd() != true);

  Big_model->set_model_setting_for_model_from_file(Input_Info);
  // /////////////////////////////////////////////

  Dialog_open_model my_dialog_for_open_model(Big_model, false);

  my_dialog_for_open_model.setModal(true);

  my_dialog_for_open_model.exec();

  this->menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(true);
}

void simbad::gui::MainWindow::clean_Big_model()
{
  delete this->Big_model;
  this->Big_model = new Model_of_space;
}

void simbad::gui::MainWindow::on_actionOpen_Initial_Configuration_triggered()
{
  clean_init_conf_and_download_model_settings();
  QFile file;
  QString File_name = QFileDialog::getOpenFileName(
      this, tr("Open file for initial configuration"), "/home",
      tr("Model (*.cnf)"));

  file.setFileName(File_name);
  if(File_name != "" && file.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    QTextStream input(&file);

    QStringList Input_Info;

    do
    {
      Input_Info.push_back(input.readLine());
    } while(input.atEnd() != true);

    if(Big_model->set_configiguration_setting_from_file(Input_Info))
    {
      this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(true);
      Big_model->set_conf_full_file_name(File_name);
    }
    else
    {
    };
  };
}
