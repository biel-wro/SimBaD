#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <dialog_open_model.h>
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
    Dialog_open_model my_dialog_for_open_model;
    my_dialog_for_open_model.setModal(true);
    my_dialog_for_open_model.exec();
}
