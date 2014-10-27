#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <model_of_space.h>

namespace Ui {
class MainWindow;
}
namespace simbad{
namespace gui{


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //random_device RandomSeed;
    Model_of_space *Big_model;
    void clean_Big_model();
private slots:


    void on_actionDownload_Evolutiion_triggered();

    void on_actionNew_triggered();

    void on_actionNew_Evolution_triggered();

    void on_actionCreat_triggered();


    void on_actionChange_project_triggered();

private:
    Ui::MainWindow *ui;


};

}
}
#endif // MAINWINDOW_H


