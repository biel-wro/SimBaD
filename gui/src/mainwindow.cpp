#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <dialog_open_model.h>
#include <simulation_dialog.h>
#include <QFileDialog>


namespace simbad{
namespace gui{

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  ,ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
 //   MyCustomWidget *frame = new MyCustomWidget(this);
 //  ui->scrollAreaWidgetContents_2->addWidget(frame);
}

void MainWindow::on_actionDownload_Evolutiion_triggered()
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


}
}

void simbad::gui::MainWindow::on_actionNew_Evolution_triggered()
{

        Dialog_open_model my_dialog_for_open_model;
        my_dialog_for_open_model.setModal(true);
        my_dialog_for_open_model.exec();

}
