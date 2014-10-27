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


//    QFileDialog dialog(this);
//    dialog.setFileMode(QFileDialog::ExistingFile);
//    dialog.setNameFilter(tr("Model Files (*.sim)"));
//    dialog.setViewMode(QFileDialog::Detail);

//    QStringList fileNames;
//    if (dialog.exec()){

        //fileNames = dialog.selectedFiles();

        //Dialog_open_model my_dialog_for_open_model;
        //my_dialog_for_open_model.setModal(true);
        //my_dialog_for_open_model.exec();
    //}

    QString dir = QFileDialog::getExistingDirectory(this, tr("Select project directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
        //найти файлы с расширением *.sim если файлов не будет сообщить об этом если будет 2 то тоже сообщить об этом
    QFile file;
    //QDir::setCurrent(dir);
    QDir Dir;
    Dir.setCurrent(dir);
    Dir.
    dir = dir + "/" + Big_model->get_name_of_model();
    Dir.mkpath(Big_model->get_name_of_model());
    QDir::setCurrent(dir);

    file.setFileName(Big_model->get_name_of_model());
    //QDir::setCurrent("/home");
    //file.open(QIODevice::ReadOnly);
    //file.open(QIODevice::ReadOnly);

    //QFile file;
    //(Big_model->get_name_of_model())
    if (file.exists()) {
        file.remove();
        file.setFileName(Big_model->get_name_of_model());
    };
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
       return;

     QTextStream out(&file);
     QStringList File_info = Big_model->get_model_settings_for_file();

     for (auto it = std::begin(File_info); it!=std::end(File_info); ++it)
        out << *it;



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
        clean_Big_model();
        Big_model->set_name_of_model("New_model");
        Dialog_open_model my_dialog_for_open_model(Big_model,true);

//save refferens to mainForm for Big_model "main model"


//        my_dialog_for_open_model.Big_model=this->Big_model;

        my_dialog_for_open_model.setModal(true);

        my_dialog_for_open_model.exec();

        //this->menuBar()->actions()[0]->menu()->actions()[0]->setEnabled(false);
        //this->menuBar()->actions()[1]->menu()->actions()[0]->setEnabled(true);
        //this->menuBar()->actions()[1]->menu()->actions()[1]->setEnabled(true);
        this->menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(true);
        //this->menuBar()->actions()[0]->menu()->actions()[4]->setEnabled(true);


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


void simbad::gui::MainWindow::on_actionChange_project_triggered()
{
    Dialog_open_model my_dialog_for_open_model(Big_model,false);

   // my_dialog_for_open_model.Big_model=this->Big_model;

    my_dialog_for_open_model.setModal(true);

    my_dialog_for_open_model.exec();
}

void simbad::gui::MainWindow::clean_Big_model()
{
    delete this->Big_model;
    this->Big_model = new Model_of_space;
}
