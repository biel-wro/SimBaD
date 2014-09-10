#ifndef SIMULATION_DIALOG_H
#define SIMULATION_DIALOG_H

#include <QDialog>

namespace Ui {
class Simulation_Dialog;
}

class Simulation_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Simulation_Dialog(QWidget *parent = 0);
    ~Simulation_Dialog();

    void initializeGL();
    void paintGL();
    void resize(int w,int h);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Simulation_Dialog *ui;

};

#endif // SIMULATION_DIALOG_H
