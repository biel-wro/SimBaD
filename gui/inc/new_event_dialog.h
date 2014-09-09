#ifndef NEW_EVENT_DIALOG_H
#define NEW_EVENT_DIALOG_H

#include <QDialog>
#include "model_of_space.h"
#include "dialog_open_model.h"
#include "event_of_model.h"

//
//
//#include "mainwindow.h"
//#include "model_of_space.h"
//
//

namespace Ui {
class New_event_dialog;
}


namespace simbad{
namespace gui{

class New_event_dialog : public QDialog
{
    Q_OBJECT

    int current_number_of_event_for_changing;
public:

//
//
//    save refferens to mainForm for Big_model "main model"
//    simbad::gui::MainWindow *mainForm;
//
//
    Model_of_space *Big_model;
//    Ui::Dialog_open_model *Ui_for_parent_Dialog_open_model;

    explicit New_event_dialog(QWidget *parent = 0);
    ~New_event_dialog();
//    void set_number_of_types_and_probable_events(int Number=0);
    void initialisation_of_New_event_dialog();
    void initialisation_of_changing_event_dialog(int Number_of_event);

private slots:

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:

    Ui::New_event_dialog *ui;

    bool New_event_dialog_regime;

};

}
}
#endif // NEW_EVENT_DIALOG_H
