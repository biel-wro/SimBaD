#include "dialog_for_init_model.h"
#include "ui_dialog_for_init_model.h"
#include <QColorDialog>
#include "glwidjet_for_init_model.h"
#include "qfiledialog.h"
using namespace std;
#include<qstring.h>

simbad::gui::Dialog_for_init_model::Dialog_for_init_model(bool new_configuration, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_for_init_model)
{




    ui->setupUi(this);
    new_config = new_configuration;
    if (new_config) this->ui->pushButton->setText("Save new config ...");
            else this->ui->pushButton->setText("Save configuration");
    //this->ui->widget->RandomSeed=this->RandomSeed;

    QPalette Pal_color_for_init_conf(palette());
    // set black background
    Pal_color_for_init_conf.setColor(QPalette::Background, Qt::black);
    this->ui->widget_color_for_init_conf->setAutoFillBackground(true);
    this->ui->widget_color_for_init_conf->setPalette(Pal_color_for_init_conf);

    QPalette Pal_color_for_dynamic_sim(palette());
    // set black background
    Pal_color_for_dynamic_sim.setColor(QPalette::Background, Qt::gray);
    this->ui->widget_color_for_dynamic_sim->setAutoFillBackground(true);
    this->ui->widget_color_for_dynamic_sim->setPalette(Pal_color_for_dynamic_sim);


    ui->widget->setParentWidjetpos(this->pos());
 //   ui->label->setText(QString::number(this->Big_model->ModelPoints.Vector_of_types.));
    ui->radioButton->setChecked(true);
    ui->widget->set_regime(true);
}

simbad::gui::Dialog_for_init_model::~Dialog_for_init_model()
{
    delete ui;
}

void simbad::gui::Dialog_for_init_model::on_pushButton_color_for_init_conf_clicked()
{
    QColorDialog Color_Dialog;
    QColor Color;
    Color = Color_Dialog.getColor(Qt::white,0 , "Choose the color of the particles for the initial configuration", 0);



    QPalette Pal_color_for_init_conf(palette());
    // set background
    Pal_color_for_init_conf.setColor(QPalette::Background, Color);
    this->ui->widget_color_for_init_conf->setAutoFillBackground(true);
    this->ui->widget_color_for_init_conf->setPalette(Pal_color_for_init_conf);

    //set color in GLwidjet
    this->ui->widget->setCurrent_color_for_init_conf(Color);
    this->Big_model->ModelPoints.
            Vector_of_types[this->ui->comboBox_Type_of_particle->currentIndex()].
            InitialConfigurationColor=Color;

}

void simbad::gui::Dialog_for_init_model::on_pushButton_color_for_dynamic_sim_clicked()
{
    QColorDialog Color_Dialog;
    QColor Color;
    Color = Color_Dialog.getColor(Qt::white,0 , "Choose the color of the particles for dynamic simulation", 0);



    QPalette Pal_color_for_dynamic_sim(palette());
    // set background
    Pal_color_for_dynamic_sim.setColor(QPalette::Background, Color);
    this->ui->widget_color_for_dynamic_sim->setAutoFillBackground(true);
    this->ui->widget_color_for_dynamic_sim->setPalette(Pal_color_for_dynamic_sim);

    //set color in GLwidjet
    this->ui->widget->setCurrent_color_for_dynam_conf(Color);
    this->Big_model->ModelPoints.
            Vector_of_types[this->ui->comboBox_Type_of_particle->currentIndex()].
            DynamicSimulationColor=Color;

}

void simbad::gui::Dialog_for_init_model::on_horizontalSlider_valueChanged(int value)
{
   ui->widget->setBrushRadius(value);
}

void simbad::gui::Dialog_for_init_model::moveEvent ( QMoveEvent * event)
{
  ui->widget->setParentWidjetpos(this->pos());
}
void simbad::gui::Dialog_for_init_model::setModel_of_space(Model_of_space * Model)
{
     Big_model = Model;
     ui->widget->Big_model = Model;

     QString String_for_particle_types ;
     for (int i=1;i <= Big_model->get_number_of_types(); i++){
         String_for_particle_types = "Particles with type # " + QString::number(i);
         ui->comboBox_Type_of_particle->addItem(String_for_particle_types );
     };
}

void simbad::gui::Dialog_for_init_model::on_comboBox_Type_of_particle_currentIndexChanged(int index)
{
    this->ui->widget->setCurrent_type(index);
    this->ui->widget->setCurrent_color_for_init_conf(this->Big_model->ModelPoints.
                                                     Vector_of_types[this->ui->comboBox_Type_of_particle->currentIndex()].
                                                    InitialConfigurationColor);
    this->ui->widget->setCurrent_color_for_dynam_conf(this->Big_model->ModelPoints.
                                                      Vector_of_types[this->ui->comboBox_Type_of_particle->currentIndex()].
            DynamicSimulationColor);
    QPalette Pal_color_for_init_conf(palette());
    // set black background
    Pal_color_for_init_conf.setColor(QPalette::Background,  this->Big_model->ModelPoints.
             Vector_of_types[this->ui->comboBox_Type_of_particle->currentIndex()].
            InitialConfigurationColor);
    this->ui->widget_color_for_init_conf->setAutoFillBackground(true);
    this->ui->widget_color_for_init_conf->setPalette(Pal_color_for_init_conf);


    QPalette Pal_color_for_dynamic_sim(palette());
    // set black background
    Pal_color_for_dynamic_sim.setColor(QPalette::Background,  this->Big_model->ModelPoints.
                                       Vector_of_types[this->ui->comboBox_Type_of_particle->currentIndex()].
                                       DynamicSimulationColor);
    this->ui->widget_color_for_dynamic_sim->setAutoFillBackground(true);
    this->ui->widget_color_for_dynamic_sim->setPalette(Pal_color_for_dynamic_sim);



}



void simbad::gui::Dialog_for_init_model::on_horizontalSlider_2_valueChanged(int value)
{
    this->ui->widget->setIntensityValue(value);
}

void simbad::gui::Dialog_for_init_model::on_pushButton_clicked()
{
  if (new_config){
    QFile file;
    QString File_name;
    File_name =         QFileDialog::getSaveFileName(this,
                                                     tr("Save configuration file "),
                                                     "new_configuration",
                                                     tr("Model (*.cnf)"));
    file.setFileName(File_name);
    if (file.exists()) {
        file.remove();
        file.setFileName(File_name);
    };
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    return;


    QTextStream out(&file);
    QStringList File_info = Big_model->get_configuration_setting_for_file();
    for (auto it = std::begin(File_info); it!=std::end(File_info); ++it)
    out << *it;

    this->close();
 } else {
     //QFile file;
     //file.setFileName(Big_model->get_full_file_name());

     //if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
     //return;

     //QTextStream out(&file);
     //QStringList File_info = Big_model->get_model_settings_for_file();

     //for (auto it = std::begin(File_info); it!=std::end(File_info); ++it)
     //out << *it;
    this->close();
 };





}


void simbad::gui::Dialog_for_init_model::on_radioButton_clicked(bool checked)
{
    this->ui->widget->set_regime(checked);
}
