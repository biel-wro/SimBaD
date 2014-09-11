#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <dialog_open_model.h>
#include <simulation_dialog.h>
#include <QFileDialog>
#include <model_of_space.h>



simbad::gui::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  ,ui(new Ui::MainWindow)
{
//Big_model
 //   Model_of_space A;

    this->Big_model = new Model_of_space;

    ui->setupUi(this);
}

simbad::gui::MainWindow::~MainWindow()
{
//    Model_of_space *A;
//    A=;
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

    Simulation_Dialog my_dialog_for_open_model;

    my_dialog_for_open_model.setModal(true);
    my_dialog_for_open_model.exec();


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
}



void simbad::gui::MainWindow::on_pushButton_clicked()
{

}

void simbad::gui::MainWindow::on_actionCreat_triggered()
{
    Simulation_Dialog Sim_dialog_for_new_conf;
    Sim_dialog_for_new_conf.setModal(true);
    Sim_dialog_for_new_conf.exec();

}
