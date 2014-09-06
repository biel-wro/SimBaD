#include "dialog_open_model.h"
#include "ui_dialog_open_model.h"
#include <QFileDialog>
#include "new_event_dialog.h"



simbad::gui::Dialog_open_model::Dialog_open_model(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_open_model)
{
    ui->setupUi(this);
    ui->tableWidget->selectRow(0);

//    we have mistake here ->
//    this->ui->spinBox_2->setValue(3);

}

simbad::gui::Dialog_open_model::~Dialog_open_model()
{
    delete ui;
}



void simbad::gui::Dialog_open_model::on_pushButton_4_clicked()
{
    New_event_dialog my_New_event_dialog;

//save refferens to  Big_model "main model"

    my_New_event_dialog.Big_model=this->Big_model;

    my_New_event_dialog.Big_model->set_number_of_types(this->ui->spinBox_2->value());

    my_New_event_dialog.initialisation_of_New_event_dialog();






//
//
//    my_New_event_dialog.mainForm->Big_model.set_number_of_types(this->ui->spinBox_2->value());
//
//

    my_New_event_dialog.setModal(true);
    my_New_event_dialog.exec();

}

void simbad::gui::Dialog_open_model::on_spinBox_2_valueChanged(int arg1)
{
    this->Big_model->set_number_of_types(arg1);
}


void simbad::gui::Dialog_open_model::on_pushButton_5_clicked()
{
    this->ui->label_7->setText(QString::number(this->Big_model->get_number_of_types()));

}


