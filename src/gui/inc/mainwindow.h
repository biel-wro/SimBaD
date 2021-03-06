#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Model/model_of_space.h>

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
    void clean_init_conf_and_download_model_settings();
private slots:


    void on_actionDownload_Evolutiion_triggered();

    void on_actionNew_triggered();

    void on_actionNew_Evolution_triggered();

    void on_actionCreat_triggered();


    void on_actionChange_project_triggered();


    void on_actionOpen_Initial_Configuration_triggered();

private:
    Ui::MainWindow *ui;


};

}
}
#endif // MAINWINDOW_H


