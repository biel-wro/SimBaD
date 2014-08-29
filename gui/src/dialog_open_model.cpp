#include "dialog_open_model.h"
#include "ui_dialog_open_model.h"
#include <QFileDialog>
#include "new_event_dialog.h"


Dialog_open_model::Dialog_open_model(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_open_model)
{
    ui->setupUi(this);
    ui->tableWidget->selectRow(0);


}

Dialog_open_model::~Dialog_open_model()
{
    delete ui;
}



void Dialog_open_model::on_pushButton_4_clicked()
{
    New_event_dialog my_New_event_dialog;
    my_New_event_dialog.set_number_of_types_and_probable_events(this->ui->spinBox_2->value());

    my_New_event_dialog.setModal(true);
    my_New_event_dialog.exec();

}
