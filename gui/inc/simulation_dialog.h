#ifndef SIMULATION_DIALOG_H
#define SIMULATION_DIALOG_H

#include <QDialog>
#include "model_of_space.h"

namespace Ui {
class Simulation_Dialog;
}



namespace simbad{
namespace gui{




class Simulation_Dialog : public QDialog
{
    Q_OBJECT

public:
    //bool sim_regime=false;
    Model_of_space* Big_model;
    explicit Simulation_Dialog(QWidget *parent = 0);
    ~Simulation_Dialog();
    void setModel_of_space(Model_of_space * Model);
    int step_for_number_of_event=0;

private slots:

    void on_pushButton_clicked();

    void on_spinBox_valueChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void dynamics();
    void on_horizontalSlider_valueChanged(int value);
    
    void on_verticalScrollBar_valueChanged(int value);

    void on_horizontalScrollBar_valueChanged(int value);

private:
    Ui::Simulation_Dialog *ui;
    QTimer *Timer_clock;

};

}//namespace simbad{
}//namespace gui{

#endif // SIMULATION_DIALOG_H
