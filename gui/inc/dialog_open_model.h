#ifndef DIALOG_OPEN_MODEL_H
#define DIALOG_OPEN_MODEL_H

#include <QDialog>

namespace Ui {
class Dialog_open_model;
}

class Dialog_open_model : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_open_model(QWidget *parent = 0);
    ~Dialog_open_model();

private slots:

    void on_pushButton_4_clicked();

private:
    Ui::Dialog_open_model *ui;
};

#endif // DIALOG_OPEN_MODEL_H
