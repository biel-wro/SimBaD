#include "new_event_dialog.h"
#include "dialog_open_model.h"
#include "event_of_model.h"
#include "ui_new_event_dialog.h"
#include <QDoubleSpinBox>
#include <QtCore>
#include <QtMath>
#include <iostream>
#include <mainwindow.h>
#include <model_of_space.h>

using namespace std;

namespace simbad
{
namespace gui
{
New_event_dialog::New_event_dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::New_event_dialog)
{
  ui->setupUi(this);
  New_event_dialog_regime = true;
}

void New_event_dialog::initialisation_of_New_event_dialog()
{
  ui->comboBox->clear();

  int number_of_types;

  number_of_types = this->Big_model->get_number_of_types();

  QString String_for_birth_death_jump_of_1_particle;
  //    Event_of_model Current_event_1;
  for(int i = 1; i <= number_of_types; i++)
  {
    // Environment
    //-- birth
    String_for_birth_death_jump_of_1_particle =
        "birth of one particle with type # " + QString::number(i) +
        " from an environment";
    add_string_to_com_box(String_for_birth_death_jump_of_1_particle);

    // birth-------------------------------------------------------------------------------------------------------
    for(int j = 1; j <= number_of_types; j++)
    {
      String_for_birth_death_jump_of_1_particle =
          "birth of one particle with type # " + QString::number(i) +
          " from particle with type # " + QString::number(j);
      add_string_to_com_box(String_for_birth_death_jump_of_1_particle);
    };
    //-------------------------------------------------------------------------------------------------------

    // death-------------------------------------------------------------------------------------------------------

    String_for_birth_death_jump_of_1_particle =
        "death of one particle with type # " + QString::number(i);

    add_string_to_com_box(String_for_birth_death_jump_of_1_particle);

    //-------------------------------------------------------------------------------------------------------

    // jump-------------------------------------------------------------------------------------------------------

    String_for_birth_death_jump_of_1_particle =
        "jump of one particle with type # " + QString::number(i);
    add_string_to_com_box(String_for_birth_death_jump_of_1_particle);

    //-------------------------------------------------------------------------------------------------------
  };
  // mutation----------------------------------------------------------------------------------
  QString String_for_particle_mutations;
  if(number_of_types > 1)
  {
    for(int i = 1; i <= number_of_types; i++)
      for(int j = 1; j <= number_of_types; j++)
      {
        if(i != j)
        {
          String_for_particle_mutations =
              "mutation of one particle with type # " + QString::number(i) +
              " to particle with type # " + QString::number(j);

          add_string_to_com_box(String_for_particle_mutations);
        };
      };
  };
  //-------------------------------------------------------------------
}

void simbad::gui::New_event_dialog::add_string_to_com_box(
    QString String_for_com_box)
{
  Event_of_model Current_event;
  Current_event.set_name_of_event(String_for_com_box);
  Current_event.set_type_of_event(String_for_com_box);

  QVector<Event_of_model>::const_pointer Const_Pointer =
      std::find(this->Big_model->List_of_events_of_model.begin(),
                this->Big_model->List_of_events_of_model.end(), Current_event);

  if(Const_Pointer == end(this->Big_model->List_of_events_of_model))
  {
    ui->comboBox->addItem(String_for_com_box);
  }
  else
  {
  };
}

void simbad::gui::New_event_dialog::initialisation_of_changing_event_dialog(
    int Number_of_event)
{
  New_event_dialog_regime = false;

  this->current_number_of_event_for_changing = Number_of_event;

  ui->comboBox->clear();

  ui->comboBox->addItem(
      this->Big_model->List_of_events_of_model[Number_of_event]
          .get_name_of_event());

  this->ui->pushButton_2->close();
}

New_event_dialog::~New_event_dialog() { delete ui; }
void New_event_dialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
  int number_of_types;

  number_of_types = this->Big_model->get_number_of_types();

  ui->tableWidget->clear();

  if(ui->comboBox->currentText().contains(
         "birth of one particle with type #") or
     ui->comboBox->currentText().contains(
         "death of one particle with type #") or
     ui->comboBox->currentText().contains(
         "mutation of one particle with type #") or
     ui->comboBox->currentText().contains("jump of one particle with type #"))
  {
    bool influence_of_environment = false;

    if(ui->comboBox->currentText().contains(
           "death of one particle with type #") or
       ui->comboBox->currentText().contains(
           "jump of one particle with type #") or
       ui->comboBox->currentText().contains("from an environment") or
       ui->comboBox->currentText().contains(
           "mutation of one particle with type #"))
      influence_of_environment = true;

    ui->tableWidget->setEnabled(true);

    ui->label_2->setText("Table of component rates for choosen event (d is a "
                         "distance between chosen point and a neighbor point)");
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(number_of_types + 1);

    QStringList header;
    header << "type"
           << "function"
           << "multiplication \n by constant"
           << "restriction of \n the range"
           << "range of \n the function"
           << "approach \n of influence"
           << "Const. rate";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    QString my_String_for_environment;
    my_String_for_environment = "Environment";

    // if(this->New_event_dialog_regime == false)

    QTableWidgetItem *newItem_for_environment =
        new QTableWidgetItem(my_String_for_environment);
    ui->tableWidget->setItem(0, 0, newItem_for_environment);

    ui->tableWidget->setCellWidget(0, 1, new QComboBox(ui->tableWidget));
    QComboBox *pComboB_environment(
        qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(0, 1)));
    pComboB_environment->addItem("-- 0");
    pComboB_environment->addItem("-- 1");
    // Changing ragime
    if(this->New_event_dialog_regime == false)
    {
      //       ui->label->setText(
      //       this->Big_model->List_of_events_of_model[current_number_of_event_for_changing].
      //                                     Table_of_component_rates_for_event_of_model.Table[0].get_string_of_Functions()
      //                          );

      pComboB_environment->setCurrentText(
          this->Big_model
              ->List_of_events_of_model[current_number_of_event_for_changing]
              .Table_of_component_rates_for_event_of_model.Table[0]
              .get_string_of_Functions());
    };
    // ---- Changing ragime

    for(int i = 1; i <= number_of_types; i++)
    {
      QString my_String = "Particles of ";
      my_String = my_String + QString::number(i) + "-type";
      QTableWidgetItem *newItem = new QTableWidgetItem(my_String);
      ui->tableWidget->setItem(i, 0, newItem);
    };

    for(int i = 1; i <= number_of_types; i++)
    {
      ui->tableWidget->setCellWidget(i, 1, new QComboBox(ui->tableWidget));
      QComboBox *pComboB(
          qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(i, 1)));
      pComboB->addItem("-- 0");
      pComboB->addItem(
          "-- 1 - d if d \u2208 [0, 1),\n    0 if d \u2208 [1, \u221E)");
      pComboB->addItem("-- d if d \u2208 [0; +\u221E)");
      pComboB->addItem("-- 1");
      pComboB->addItem("-- exp(-d) if d \u2208 [0; +\u221E)");
      //        pComboB->addItem("-- 0 if d \u2208 [0,1)\n    d - 1 if d \u2208
      //        [1; +\u221E)"); pComboB->addItem("-- 0 if d \u2208 [0,1)\n    d
      //        - 1 if d \u2208 [1; +\u221E)"); pComboB->addItem("-- min(d)");

      if(this->New_event_dialog_regime == false)
      {
        pComboB->setCurrentText(
            this->Big_model
                ->List_of_events_of_model[current_number_of_event_for_changing]
                .Table_of_component_rates_for_event_of_model.Table[i]
                .get_string_of_Functions());
      };
    };

    for(int i = 0; i <= number_of_types; i++)
    {
      ui->tableWidget->setCellWidget(i, 2, new QDoubleSpinBox(ui->tableWidget));

      if(this->New_event_dialog_regime == false)
      {
        QDoubleSpinBox *QDSpinBox(
            qobject_cast<QDoubleSpinBox *>(ui->tableWidget->cellWidget(i, 2)));
        QDSpinBox->setValue(
            (double)this->Big_model
                ->List_of_events_of_model[current_number_of_event_for_changing]
                .Table_of_component_rates_for_event_of_model.Table[i]
                .get_Multiplication_of_constant());
      };
    };

    ui->tableWidget->setCellWidget(0, 3, new QComboBox(ui->tableWidget));
    QComboBox *pComboB_Restrictions(
        qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(0, 3)));
    //+++
    //        pComboB->addItem("zero");
    pComboB_Restrictions->addItem("--");
    //        pComboB->addItem("square");
    //+++
    if(this->New_event_dialog_regime == false)
    {
      // pComboB->setCurrentText("square");
      pComboB_Restrictions->setCurrentText(
          this->Big_model
              ->List_of_events_of_model[current_number_of_event_for_changing]
              .Table_of_component_rates_for_event_of_model.Table[0]
              .get_string_of_Restrictions());
    };

    for(int i = 1; i <= number_of_types; i++)
    {
      ui->tableWidget->setCellWidget(i, 3, new QComboBox(ui->tableWidget));
      QComboBox *pComboB(
          qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(i, 3)));
      //+++
      //        pComboB->addItem("zero");
      pComboB->addItem("circle");
      //        pComboB->addItem("square");
      //+++
      if(this->New_event_dialog_regime == false)
      {
        // pComboB->setCurrentText("square");
        pComboB->setCurrentText(
            this->Big_model
                ->List_of_events_of_model[current_number_of_event_for_changing]
                .Table_of_component_rates_for_event_of_model.Table[i]
                .get_string_of_Restrictions());
      };
    };

    for(int i = 0; i <= number_of_types; i++)
    {
      ui->tableWidget->setCellWidget(i, 4, new QDoubleSpinBox(ui->tableWidget));
      if(this->New_event_dialog_regime == false)
      {
        QDoubleSpinBox *QDSpinBox(
            qobject_cast<QDoubleSpinBox *>(ui->tableWidget->cellWidget(i, 4)));
        QDSpinBox->setValue(
            (double)this->Big_model
                ->List_of_events_of_model[current_number_of_event_for_changing]
                .Table_of_component_rates_for_event_of_model.Table[i]
                .get_Range_of_function());
      };

      // QDoubleSpinBox
      // *pDoubleSpinB(qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(i,2)));
    };

    ui->tableWidget->setCellWidget(0, 5, new QComboBox(ui->tableWidget));
    QComboBox *pEnvironmentComboB(
        qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(0, 5)));
    pEnvironmentComboB->addItem("-");

    for(int i = 1; i <= number_of_types; i++)
    {
      ui->tableWidget->setCellWidget(i, 5, new QComboBox(ui->tableWidget));
      QComboBox *pComboB(
          qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(i, 5)));
      //++++++++++++++++++++++++++++++++++++++
      //        pComboB->addItem("no");
      pComboB->addItem("\u2211");
      //        pComboB->addItem("exp(-\u2211)");
      //        pComboB->addItem("exp(+\u2211)");
      //        pComboB->addItem("\u220F");
      //++++++++++++++++++++++++++++++++++++++
      if(this->New_event_dialog_regime == false)
      {
        pComboB->setCurrentText(
            this->Big_model
                ->List_of_events_of_model[current_number_of_event_for_changing]
                .Table_of_component_rates_for_event_of_model.Table[i]
                .get_string_of_Approach_of_influence());
      };
    };

    for(int i = 0; i <= number_of_types; i++)
    {
      ui->tableWidget->setCellWidget(i, 6, new QDoubleSpinBox(ui->tableWidget));
      if(this->New_event_dialog_regime == false)
      {
        QDoubleSpinBox *QDSpinBox(
            qobject_cast<QDoubleSpinBox *>(ui->tableWidget->cellWidget(i, 6)));
        QDSpinBox->setValue(
            (double)this->Big_model
                ->List_of_events_of_model[current_number_of_event_for_changing]
                .Table_of_component_rates_for_event_of_model.Table[i]
                .get_Constant_rate_plus());
      };
    };

    // Environment++++++++
    // ///////////////////////////////////////// //
    //                                           //
    if(influence_of_environment == false)
    {
      pComboB_environment->setEnabled(false);

      QDoubleSpinBox *QDSpinBox_Mul_by_const(
          qobject_cast<QDoubleSpinBox *>(ui->tableWidget->cellWidget(0, 2)));
      QDSpinBox_Mul_by_const->setEnabled(false);
    };

    // QComboBox
    // *pComboB_Restrictions(qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(0,3)));
    pComboB_Restrictions->setEnabled(false);

    QDoubleSpinBox *QDSpinBox_Range_of_function(
        qobject_cast<QDoubleSpinBox *>(ui->tableWidget->cellWidget(0, 4)));
    QDSpinBox_Range_of_function->setEnabled(false);

    QComboBox *pComboB_Approach_of_influence(
        qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(0, 5)));
    pComboB_Approach_of_influence->setEnabled(false);

    QDoubleSpinBox *QDSpinBox_Constant_rate_plus(
        qobject_cast<QDoubleSpinBox *>(ui->tableWidget->cellWidget(0, 6)));
    QDSpinBox_Constant_rate_plus->setEnabled(false);

    if(ui->comboBox->currentText().contains(
           "birth of one particle with type #") &&
       ui->comboBox->currentText().contains("from an environment"))
    {
      pComboB_Restrictions->setEnabled(true);

      pComboB_Restrictions->addItem("square");
      pComboB_Restrictions->setCurrentText("square");
      QDSpinBox_Range_of_function->setEnabled(true);

      for(int i = 1; i <= number_of_types; i++)
        for(int j = 1; j <= 6; j++)
          ui->tableWidget->cellWidget(i, j)->setEnabled(false);
    };

    //                                                    //
    // ////////////////////////////////////////////////// //

    // Exclude++++++++++++++++++++++++++++++++++
    //      QDoubleSpinBox*
    //      QDSpinBox(qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(0,6)));
    //      QDSpinBox->setMinimum(0.0);
    //      QDSpinBox->setMaximum(0.0);
    //+++++++++++++++++++++++++++++++++++++++++

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(
        1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(
        3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(
        4, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(
        5, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(
        6, QHeaderView::ResizeToContents);
  }
  else
  {
    ui->tableWidget->setEnabled(false);
  };

  if((ui->comboBox->currentText().contains(
          "birth of one particle with type #") or
      ui->comboBox->currentText().contains(
          "jump of one particle with type")) and
     (ui->comboBox->currentText().contains("from an environment") != true))
  {
    ui->tableWidget_2->setEnabled(true);
    ui->label_3->setEnabled(true);
    ui->label_3->setText("2-dim Student's t - distribution of a new particle "
                         "with scale paramiter \u03C3 (0 is for Gaussian)");

    ui->tableWidget_2->setColumnCount(3);

    ui->tableWidget_2->setRowCount(1);

    QStringList header;
    header << "Type of event "
           << "Coefficient of t - distribution"
           << "\u03C3 ";
    //\n(X=\u03C3 Z; \n Z- standart t-student distribution)
    ui->tableWidget_2->setHorizontalHeaderLabels(header);

    QTableWidgetItem *newItem_for_environment =
        new QTableWidgetItem(ui->comboBox->currentText());
    ui->tableWidget_2->setItem(0, 0, newItem_for_environment);

    ui->tableWidget_2->setCellWidget(0, 1, new QSpinBox(ui->tableWidget));
    ui->tableWidget_2->setCellWidget(0, 2, new QDoubleSpinBox(ui->tableWidget));

    QDoubleSpinBox *QDoubleBox_for_scale_paramiter(
        qobject_cast<QDoubleSpinBox *>(ui->tableWidget_2->cellWidget(0, 2)));
    QDoubleBox_for_scale_paramiter->setValue(0.1);
    QDoubleBox_for_scale_paramiter->setMinimum(0.01);

    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(
        1, QHeaderView::ResizeToContents);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::ResizeToContents);

    if(this->New_event_dialog_regime == false)
    {
      QSpinBox *QSpinBox_for_distribution(
          qobject_cast<QSpinBox *>(ui->tableWidget_2->cellWidget(0, 1)));
      QSpinBox_for_distribution->setValue(
          this->Big_model
              ->List_of_events_of_model[current_number_of_event_for_changing]
              .Distribution_of_particle_for_event
              .get_Type_of_T_student_distribution());

      QDoubleSpinBox *QDoubleBox_for_distribution(
          qobject_cast<QDoubleSpinBox *>(ui->tableWidget_2->cellWidget(0, 2)));
      QDoubleBox_for_distribution->setValue(
          this->Big_model
              ->List_of_events_of_model[current_number_of_event_for_changing]
              .Distribution_of_particle_for_event.get_scale_paramiter());
      QDoubleBox_for_distribution->setMinimum(0.01);
    };
  }
  else
  {
    ui->tableWidget_2->setEnabled(false);
    ui->tableWidget_2->clear();

    ui->label_3->setEnabled(false);
    ui->label_3->setText("Distribution of new particle");

    ui->tableWidget_2->setColumnCount(0);
    ui->tableWidget_2->setRowCount(0);
  };
}

void set_settings_for_birth_from_enviroment() {}
//++
void simbad::gui::New_event_dialog::on_pushButton_clicked()
{
  if(New_event_dialog_regime == true)
  {
    if(this->ui->comboBox->currentText() != "")
    {
      Event_of_model Current_event(this->Big_model->get_number_of_types());
      Current_event.set_name_of_event(this->ui->comboBox->currentText());
      Current_event.set_type_of_event(this->ui->comboBox->currentText());
      Current_event.set_number_of_types_for_event_of_model(
          this->Big_model->get_number_of_types());
      Current_event.set_code_of_event(this->ui->comboBox->currentText());

      Current_event.Table_of_component_rates_for_event_of_model
          .Number_of_types = this->Big_model->get_number_of_types();

      for(int i = 0; i <= this->Big_model->get_number_of_types(); i++)
      {
        function_for_component_rate Function_of_com_rate;

        Function_of_com_rate.set_influent_particle_type(i);
        QComboBox *combo_for_Functions =
            (QComboBox *)ui->tableWidget->cellWidget(i, 1);
        Function_of_com_rate.set_Functions_from_string(
            combo_for_Functions->currentText());

        QDoubleSpinBox *double_spin_box_for_Multiplication_by_constant =
            (QDoubleSpinBox *)ui->tableWidget->cellWidget(i, 2);
        Function_of_com_rate.set_Multiplication_by_constant(
            (float)double_spin_box_for_Multiplication_by_constant->value());

        QComboBox *combo_for_Restrictions =
            (QComboBox *)ui->tableWidget->cellWidget(i, 3);
        Function_of_com_rate.set_Restrictions_from_string(
            combo_for_Restrictions->currentText());
        // ui->label->setText("--"+combo_for_Restrictions->currentText()+"__");
        // ui->label->setText(Function_of_com_rate.get_string_of_Restrictions());

        QDoubleSpinBox *double_spin_box_for_Range_of_the_function =
            (QDoubleSpinBox *)ui->tableWidget->cellWidget(i, 4);
        Function_of_com_rate.set_Range_of_the_function(
            (float)double_spin_box_for_Range_of_the_function->value());

        QComboBox *combo_for_Approach_of_influence =
            (QComboBox *)ui->tableWidget->cellWidget(i, 5);
        Function_of_com_rate.set_Approach_of_influence_from_string(
            combo_for_Approach_of_influence->currentText());

        QDoubleSpinBox *double_spin_box_for_Constant_rate_plus =
            (QDoubleSpinBox *)ui->tableWidget->cellWidget(i, 6);
        Function_of_com_rate.set_Constant_rate_plus(
            (float)double_spin_box_for_Constant_rate_plus->value());

        Current_event.Table_of_component_rates_for_event_of_model.Table
            .push_back(Function_of_com_rate);
      };

      if((ui->comboBox->currentText().contains(
              "birth of one particle with type #") or
          ui->comboBox->currentText().contains(
              "jump of one particle with type")) and
         (ui->comboBox->currentText().contains("from an environment") != true))
      {
        QSpinBox *QSpin_for_T_distribution =
            (QSpinBox *)ui->tableWidget_2->cellWidget(0, 1);
        Current_event.Distribution_of_particle_for_event
            .set_Type_of_Tstudent_distribution(
                QSpin_for_T_distribution->value());

        QDoubleSpinBox *QDouble_for_scale_paramiter =
            (QDoubleSpinBox *)ui->tableWidget_2->cellWidget(0, 2);
        Current_event.Distribution_of_particle_for_event.set_scale_paramiter(
            QDouble_for_scale_paramiter->value());
      };

      // if event was in vector, then we do not do anything
      QVector<Event_of_model>::const_pointer p = std::find(
          this->Big_model->List_of_events_of_model.begin(),
          this->Big_model->List_of_events_of_model.end(), Current_event);
      ;

      if(p == end(this->Big_model->List_of_events_of_model))
      {
        this->Big_model->push_back_new_event(Current_event);
      }
      else
      {
      };

      this->initialisation_of_New_event_dialog();
    };
  }
  else
  {
    int number_of_types;

    number_of_types = this->Big_model->get_number_of_types();

    if(ui->comboBox->currentText().contains(
           "birth of one particle with type #") or
       ui->comboBox->currentText().contains(
           "death of one particle with type # ") or
       ui->comboBox->currentText().contains("Mutation of #") or
       ui->comboBox->currentText().contains("jump of one particle with type"))
    {
      //         QComboBox
      //         *pComboB_environment(qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(0,1)));
      //         this->Big_model->List_of_events_of_model[current_number_of_event_for_changing].
      //         Table_of_component_rates_for_event_of_model.Table[0].
      //                 set_Functions_from_string(pComboB_environment->currentText());

      for(int i = 0; i <= number_of_types; i++)
      {
        QComboBox *pComboB(
            qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(i, 1)));
        this->Big_model
            ->List_of_events_of_model[current_number_of_event_for_changing]
            .Table_of_component_rates_for_event_of_model.Table[i]
            .set_Functions_from_string(pComboB->currentText());
      };

      for(int i = 0; i <= number_of_types; i++)
      {
        QDoubleSpinBox *QDSpinBox(
            qobject_cast<QDoubleSpinBox *>(ui->tableWidget->cellWidget(i, 2)));
        this->Big_model
            ->List_of_events_of_model[current_number_of_event_for_changing]
            .Table_of_component_rates_for_event_of_model.Table[i]
            .set_Multiplication_by_constant((float)QDSpinBox->value());
      };

      for(int i = 0; i <= number_of_types; i++)
      {
        QComboBox *pComboB(
            qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(i, 3)));
        this->Big_model
            ->List_of_events_of_model[current_number_of_event_for_changing]
            .Table_of_component_rates_for_event_of_model.Table[i]
            .set_Restrictions_from_string(pComboB->currentText());
      };

      for(int i = 0; i <= number_of_types; i++)
      {
        QDoubleSpinBox *QDSpinBox(
            qobject_cast<QDoubleSpinBox *>(ui->tableWidget->cellWidget(i, 4)));
        this->Big_model
            ->List_of_events_of_model[current_number_of_event_for_changing]
            .Table_of_component_rates_for_event_of_model.Table[i]
            .set_Range_of_the_function(QDSpinBox->value());
      };
      // have changed-------1->0
      for(int i = 0; i <= number_of_types; i++)
      {
        QComboBox *pComboB(
            qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(i, 5)));
        this->Big_model
            ->List_of_events_of_model[current_number_of_event_for_changing]
            .Table_of_component_rates_for_event_of_model.Table[i]
            .set_Approach_of_influence_from_string(pComboB->currentText());
      };
      //------------------------

      for(int i = 0; i <= number_of_types; i++)
      {
        QDoubleSpinBox *QDSpinBox(
            qobject_cast<QDoubleSpinBox *>(ui->tableWidget->cellWidget(i, 6)));
        this->Big_model
            ->List_of_events_of_model[current_number_of_event_for_changing]
            .Table_of_component_rates_for_event_of_model.Table[i]
            .set_Constant_rate_plus((float)QDSpinBox->value());
      };
    };

    if((ui->comboBox->currentText().contains(
            "birth of one particle with type #") ||
        ui->comboBox->currentText().contains(
            "jump of one particle with type")) &&
       (ui->comboBox->currentText().contains("from an environment") != true))
    {
      QSpinBox *QSpinBox_for_distribution(
          qobject_cast<QSpinBox *>(ui->tableWidget_2->cellWidget(0, 1)));
      this->Big_model
          ->List_of_events_of_model[current_number_of_event_for_changing]
          .Distribution_of_particle_for_event.set_Type_of_Tstudent_distribution(
              QSpinBox_for_distribution->value());
      QDoubleSpinBox *QDoubleSpinBox_for_distribution(
          qobject_cast<QDoubleSpinBox *>(ui->tableWidget_2->cellWidget(0, 2)));
      this->Big_model
          ->List_of_events_of_model[current_number_of_event_for_changing]
          .Distribution_of_particle_for_event.set_scale_paramiter(
              QDoubleSpinBox_for_distribution->value());
    };

    this->close();
  };
}

void simbad::gui::New_event_dialog::on_pushButton_2_clicked() { this->close(); }
}
}
