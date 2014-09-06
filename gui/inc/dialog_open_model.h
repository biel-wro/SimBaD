#ifndef DIALOG_OPEN_MODEL_H
#define DIALOG_OPEN_MODEL_H

#include <QDialog>
#include "mainwindow.h"
#include "model_of_space.h"

namespace Ui {
class Dialog_open_model;
}

namespace simbad{
namespace gui{

class Dialog_open_model : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_open_model(QWidget *parent = 0);
    ~Dialog_open_model();

    //save refferens to mainForm for Big_model "main model"
    Model_of_space* Big_model;
private slots:

    void on_pushButton_4_clicked();

    void on_spinBox_2_valueChanged(int arg1);

    void on_pushButton_5_clicked();

private:
    Ui::Dialog_open_model *ui;


};
}
}
#endif // DIALOG_OPEN_MODEL_H
