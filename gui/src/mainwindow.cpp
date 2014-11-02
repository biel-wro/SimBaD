#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <dialog_open_model.h>
#include <simulation_dialog.h>
#include <QFileDialog>
#include <model_of_space.h>
#include <dialog_for_init_model.h>
#include <random>
#include "iostream"
using namespace std;

simbad::gui::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  ,ui(new Ui::MainWindow)
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
    QString File_name=QFileDialog::getOpenFileName(this,tr("Open file"),
                                                            "/home",tr("Model (*.sim)"));
    file.setFileName(File_name);

    clean_Big_model();

    this->menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(true);


    this->menuBar()->actions()[1]->menu()->actions()[0]->setEnabled(true);
    this->menuBar()->actions()[1]->menu()->actions()[1]->setEnabled(true);

    this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(false);


    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) return;

    QTextStream input(&file);

    QStringList Input_Info;

    do
    {
       Input_Info.push_back(input.readLine());
    } while(input.atEnd()!=true);

    Big_model->set_model_setting_for_model_from_file(Input_Info);
    Big_model->set_full_file_name(file.fileName());

    this->Big_model->ModelPoints.initialisation_of_model(Big_model->get_number_of_types());


}



void simbad::gui::MainWindow::on_actionNew_triggered()
{

    this->Big_model->initiate_before_simulation();

    Simulation_Dialog Sim_dialog_for_dyn_conf;
    Sim_dialog_for_dyn_conf.setModel_of_space(this->Big_model);
    //Sim_dialog_for_new_conf.RandomSeed=this->RandomSeed;
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
        Dialog_open_model my_dialog_for_open_model(Big_model,true);

//save refferens to mainForm for Big_model "main model"

        my_dialog_for_open_model.setModal(true);

        my_dialog_for_open_model.exec();

        this->menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(true);


        this->menuBar()->actions()[1]->menu()->actions()[0]->setEnabled(true);
        this->menuBar()->actions()[1]->menu()->actions()[1]->setEnabled(true);

        this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(false);

        this->Big_model->ModelPoints.initialisation_of_model(Big_model->get_number_of_types());
}




void simbad::gui::MainWindow::on_actionCreat_triggered()
{
    Dialog_for_init_model Sim_dialog_for_new_conf(true);
    Sim_dialog_for_new_conf.setModel_of_space(this->Big_model);

    Sim_dialog_for_new_conf.setModal(true);
    Sim_dialog_for_new_conf.exec();


    this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(true);


}


void simbad::gui::MainWindow::on_actionChange_project_triggered()
{
    Dialog_open_model my_dialog_for_open_model(Big_model,false);

   // my_dialog_for_open_model.Big_model=this->Big_model;

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


    QFile file;
    QString File_name=          QFileDialog::getOpenFileName(this,tr("Open file for initial configuration"),
                                                             "/home",tr("Model (*.cnf)"));

    file.setFileName(File_name);



    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) return;

    QTextStream input(&file);

    QStringList Input_Info;

    do
    {
      Input_Info.push_back(input.readLine());
    } while(input.atEnd()!=true);



        if (Big_model->set_configiguration_setting_from_file(Input_Info)){
//            Big_model->set_full_file_name(file.fileName());

            this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(true);

        } else {


        };
}
