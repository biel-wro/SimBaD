#include "dialog_open_model.h"
#include "ui_dialog_open_model.h"

Dialog_open_model::Dialog_open_model(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_open_model)
{
    ui->setupUi(this);
}

Dialog_open_model::~Dialog_open_model()
{
    delete ui;
}
