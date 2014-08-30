#ifndef NEW_EVENT_DIALOG_H
#define NEW_EVENT_DIALOG_H

#include <QDialog>

namespace Ui {
class New_event_dialog;
}

class New_event_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit New_event_dialog(QWidget *parent = 0);
    ~New_event_dialog();
    void set_number_of_types_and_probable_events(int Number=0);

private slots:

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::New_event_dialog *ui;
    int number_of_types=0;
};

#endif // NEW_EVENT_DIALOG_H
