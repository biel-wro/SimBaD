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
    //QMessageBox::information(this,"title","Hello");
//    QString fileName;
//    fileName = QFileDialog::getOpenFileName(this,
//   tr("Open File(Model)"), "/home/", tr("Model Files (*.sim)"));


    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Model Files (*.sim)"));
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (dialog.exec()){

        fileNames = dialog.selectedFiles();

        Dialog_open_model my_dialog_for_open_model;
        my_dialog_for_open_model.setModal(true);
        my_dialog_for_open_model.exec();
    }


}



void simbad::gui::MainWindow::on_actionNew_triggered()
{


    Simulation_Dialog Sim_dialog_for_dyn_conf;
    Sim_dialog_for_dyn_conf.setModel_of_space(this->Big_model);
    //Sim_dialog_for_new_conf.RandomSeed=this->RandomSeed;
    Sim_dialog_for_dyn_conf.setModal(true);
    Sim_dialog_for_dyn_conf.exec();




}



void simbad::gui::MainWindow::on_actionNew_Evolution_triggered()
{

        Dialog_open_model my_dialog_for_open_model;

//save refferens to mainForm for Big_model "main model"
        my_dialog_for_open_model.Big_model=this->Big_model;


        my_dialog_for_open_model.setModal(true);

        my_dialog_for_open_model.exec();

        this->menuBar()->actions()[0]->menu()->actions()[0]->setEnabled(false);
        this->menuBar()->actions()[1]->menu()->actions()[0]->setEnabled(true);
        this->menuBar()->actions()[1]->menu()->actions()[1]->setEnabled(true);

        this->Big_model->ModelPoints.initialisation_of_model(Big_model->get_number_of_types());

}




void simbad::gui::MainWindow::on_actionCreat_triggered()
{
    Dialog_for_init_model Sim_dialog_for_new_conf;
    Sim_dialog_for_new_conf.setModel_of_space(this->Big_model);
    //Sim_dialog_for_new_conf.RandomSeed=this->RandomSeed;
    Sim_dialog_for_new_conf.setModal(true);
    Sim_dialog_for_new_conf.exec();

    this->menuBar()->actions()[0]->menu()->actions()[0]->setEnabled(false);
    this->menuBar()->actions()[1]->menu()->actions()[0]->setEnabled(false);
    this->menuBar()->actions()[1]->menu()->actions()[1]->setEnabled(false);
    this->menuBar()->actions()[2]->menu()->actions()[0]->setEnabled(true);

    //cout<<"void simbad::gui::MainWindow::on_actionCreat_triggered()"<<endl;
    this->Big_model->initiate_before_simulation();
}
