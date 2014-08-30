#include "new_event_dialog.h"
#include "ui_new_event_dialog.h"
#include "dialog_open_model.h"
#include <QDoubleSpinBox>
#include <QtMath>
#include <QtCore>
#include <QtScript/QScriptEngine>

New_event_dialog::New_event_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::New_event_dialog)
{

    ui->setupUi(this);
}

New_event_dialog::~New_event_dialog()
{
    delete ui;
}
void New_event_dialog::set_number_of_types_and_probable_events(int Number)
{
    number_of_types = Number;

    QString String_for_birth_of_1_particle;
    for (int i=1;i <= this->number_of_types; i++){

        String_for_birth_of_1_particle= "birth of one particle with type # " + QString::number(i);
        ui->comboBox->addItem(String_for_birth_of_1_particle);

        String_for_birth_of_1_particle= "death of one particle with type # " + QString::number(i);
        ui->comboBox->addItem(String_for_birth_of_1_particle);

        String_for_birth_of_1_particle= "jump of one particle with type # " + QString::number(i);
        ui->comboBox->addItem(String_for_birth_of_1_particle);

    };

    QString String_for_particle_mutations;
    if (number_of_types>1){
        for (int i=1;i<= number_of_types; i++)
            for (int j=1;j<= number_of_types; j++){
            if (i!=j)
            {
                  String_for_particle_mutations = "Mutation of #" + QString::number(i)+"-type particle to #" + QString::number(j) + "-type particle";
                  ui->comboBox->addItem(String_for_particle_mutations);

            };
        }

    };
}


void New_event_dialog::on_comboBox_currentIndexChanged(const QString &arg1)
{

    if (ui->comboBox->currentText().contains("birth of one particle with type #") or
            ui->comboBox->currentText().contains("death of one particle with type # ") or
                ui->comboBox->currentText().contains("Mutation of #") or
                    ui->comboBox->currentText().contains("jump of one particle with type")
            ){
      ui->label_2->setText("Table of component rates for choosen event (d is a distance between chosen point and a neighbor point)");
      ui->tableWidget->setColumnCount(6);
      ui->tableWidget->setRowCount(this->number_of_types+1);


      QStringList header;
      header << "type" << "function" << "multiplication \n by constant"  <<"restriction of \n the range" << "range of \n the function" << "approach \n of influence";
      ui->tableWidget->setHorizontalHeaderLabels(header);


      QString my_String_for_environment="Environment ";
      QTableWidgetItem *newItem_for_environment = new QTableWidgetItem(my_String_for_environment);
      ui->tableWidget->setItem(0, 0, newItem_for_environment);

      ui->tableWidget->setCellWidget ( 0, 1, new QComboBox( ui->tableWidget ) );
      QComboBox *pComboB_environment(qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(0,1)));
      pComboB_environment->addItem("1) 0");
      pComboB_environment->addItem("2) 1");


      for(int i=1; i<=this->number_of_types; i++){
          QString my_String = "Particles of ";
          my_String = my_String + QString::number(i)+"-type";
          QTableWidgetItem *newItem = new QTableWidgetItem(my_String);
          ui->tableWidget->setItem(i, 0, newItem);
      };


      for(int i=1; i<=this->number_of_types; i++){
        ui->tableWidget->setCellWidget ( i, 1, new QComboBox( ui->tableWidget ) );
        QComboBox *pComboB(qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,1)));
        pComboB->addItem("1) 0");
        pComboB->addItem("2) 1 - d if d \u2208 [0, 1),\n    0 if d \u2208 [1, \u221E)");
        pComboB->addItem("3) d if d \u2208 [0; +\u221E)");
        pComboB->addItem("4) 1");
        pComboB->addItem("5) exp(-d) if d \u2208 [0; +\u221E)");
        pComboB->addItem("6) 0 if [0,1)\n    d - 1 if d \u2208 [1; +\u221E)");
      };


      for(int i=0; i<=this->number_of_types; i++){
        ui->tableWidget->setCellWidget ( i, 2, new QDoubleSpinBox( ui->tableWidget ) );
        //QDoubleSpinBox *pDoubleSpinB(qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(i,2)));
      };

      for(int i=0; i<=this->number_of_types; i++){
        ui->tableWidget->setCellWidget ( i, 3, new QComboBox( ui->tableWidget ) );
        QComboBox *pComboB(qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,3)));
        pComboB->addItem("circle");
        pComboB->addItem("square");
      };

      for(int i=0; i<=this->number_of_types; i++){
        ui->tableWidget->setCellWidget ( i, 4, new QDoubleSpinBox( ui->tableWidget ) );
        //QDoubleSpinBox *pDoubleSpinB(qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(i,2)));
      };

      ui->tableWidget->setCellWidget ( 0, 5, new QComboBox( ui->tableWidget ) );
      QComboBox *pEnvironmentComboB(qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(0,5)));
      pEnvironmentComboB->addItem("-");

      for(int i=1; i<=this->number_of_types; i++){
        ui->tableWidget->setCellWidget ( i, 5, new QComboBox( ui->tableWidget ) );
        QComboBox *pComboB(qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,5)));
        pComboB->addItem("\u2211");
        pComboB->addItem("exp(-\u2211)");
        pComboB->addItem("exp(+\u2211)");
        pComboB->addItem("\u220F");
      };

      ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,  QHeaderView::Stretch);
      ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,  QHeaderView::ResizeToContents);
      ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,  QHeaderView::ResizeToContents);
      ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,  QHeaderView::ResizeToContents);
      ui->tableWidget->horizontalHeader()->setSectionResizeMode(4,  QHeaderView::ResizeToContents);
      ui->tableWidget->horizontalHeader()->setSectionResizeMode(5,  QHeaderView::ResizeToContents);

    };

    if (ui->comboBox->currentText().contains("birth of one particle with type #") or
            ui->comboBox->currentText().contains("jump of one particle with type")){

        ui->tableWidget_2->setEnabled(true);
        ui->label_3->setEnabled(true);
        ui->label_3->setText("2-dim T-student distribution of a new particle (0 is for Gaussian case)");


        ui->tableWidget_2-> setColumnCount(2);

        ui->tableWidget_2->setRowCount(1);


        QStringList header;
        header << "Type of a new particle " << "Coefficient of 2-dimensional \n t -student distribution";
        ui->tableWidget_2->setHorizontalHeaderLabels(header);


        QTableWidgetItem *newItem_for_environment = new QTableWidgetItem(ui->comboBox->currentText());
        ui->tableWidget_2->setItem(0, 0, newItem_for_environment);

        ui->tableWidget_2->setCellWidget ( 0, 1, new QSpinBox( ui->tableWidget ) );

        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0,  QHeaderView::Stretch);
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(1,  QHeaderView::ResizeToContents);

    } else {
        ui->tableWidget_2->setEnabled(false);
        ui->tableWidget_2->clear();
        ui->label_3->setEnabled(false);
        ui->label_3->setText("Distribution of new particles");

    };
}
