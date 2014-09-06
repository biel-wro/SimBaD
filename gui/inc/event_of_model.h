#ifndef EVENT_OF_MODEL_H
#define EVENT_OF_MODEL_H
#include <QtCore>
#include"distribution_of_particle.h"
#include "table_of_component_rates.h"



namespace Ui {
    class Event_of_model;
}


namespace simbad{
namespace gui{




class Event_of_model{
    QString Name_of_event;
    QString Code_of_event;
    QString Type_of_event;
    int number_of_types_for_event_of_model;
// here we will put vector
    Distribution_of_particle Distribution;
public:
// here we will put vector of rate tables
    Table_of_component_rates Table_of_rates;


//constructor
    Event_of_model();
    Event_of_model(int Number_of_types_in_model_for_event);
// functions for changing the Event_of_model
    void set_code_of_event(QString Code_of_event_in_model);
    void set_name_of_event(QString Name_of_event_in_model);
    void set_type_of_event(QString Type_of_event_in_model);

    void set_number_of_types_for_event_of_model(int Number);
// it should be done in *.cpp


};


}//namespace gui
}//namespace simbad


#endif // EVENT_OF_MODEL_H
