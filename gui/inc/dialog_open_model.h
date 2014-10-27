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
    explicit Dialog_open_model(Model_of_space* B_Model,bool new_model,QWidget *parent = 0);
    ~Dialog_open_model();

    //save refferens to mainForm for Big_model "main model"
    Model_of_space* Big_model;
    void initialization_of_Table();
 //   Ui::MainWindow *Ui_for_parent_Main_window;
private slots:

    void on_pushButton_4_clicked();

    void on_spinBox_2_valueChanged(int arg1);




    void on_spinBox_3_valueChanged(int arg1);

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Dialog_open_model *ui;
};
}
}
#endif // DIALOG_OPEN_MODEL_H
