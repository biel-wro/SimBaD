#ifndef DIALOG_FOR_INIT_MODEL_H
#define DIALOG_FOR_INIT_MODEL_H

#include <QDialog>
#include "model_of_space.h"


namespace Ui {
class Dialog_for_init_model;
}


namespace simbad{
namespace gui{
class Dialog_for_init_model : public QDialog
{
    Q_OBJECT
    bool new_config;
public:
    Model_of_space* Big_model;
//    random_device RandomSeed;
    explicit Dialog_for_init_model(bool new_configuration, QWidget *parent = 0);
    ~Dialog_for_init_model();
    void setModel_of_space(Model_of_space * Model);

private slots:

    void on_pushButton_color_for_init_conf_clicked();
    void on_pushButton_color_for_dynamic_sim_clicked();
    void moveEvent (QMoveEvent * event );

    void on_comboBox_Type_of_particle_currentIndexChanged(int index);

    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);

    void on_pushButton_clicked();



    void on_radioButton_clicked(bool checked);

private:
    Ui::Dialog_for_init_model *ui;
};



}//namespace simbad{
}//namespace gui{
#endif // DIALOG_FOR_INIT_MODEL_H
