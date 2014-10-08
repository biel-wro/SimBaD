#ifndef EVENT_OF_MODEL_H
#define EVENT_OF_MODEL_H
#include <QtCore>
#include "distribution_of_particle.h"
#include "table_of_component_rates.h"



namespace Ui {
    class Event_of_model;
}


namespace simbad{
namespace gui{




class Event_of_model{


    QString Name_of_event;
    //QString Code_of_event;
    QString Type_of_event;
    int Number_of_types_for_event_of_model = 1;
//Example 0->1 birth of particle, 1->0 death of particle----------

//----------------------------------------------------------------



//--------------------------------- Distribution_of_particle Distribution;

public:
    typedef QVector <int> type_of_particle_before_and_after_event;
    QVector <type_of_particle_before_and_after_event> Code_of_event;

// here we will put vector of rate tables
    Table_of_component_rates Table_of_component_rates_for_event_of_model;

// here we will put vector of Distribution of particles
    Distribution_of_particle Distribution_of_particle_for_event;

//--------------------------------- Table_of_component_rates Table_of_rates;


//constructor
    Event_of_model();
    Event_of_model(const Event_of_model& that);
    Event_of_model(int Number_of_types_in_model_for_event);
// Overloading operators
    Event_of_model& operator = (const Event_of_model& that);




    bool operator == (const Event_of_model& that)
    {
        if (       (this->Name_of_event == that.Name_of_event) and
                   //(this->Code_of_event == that.Code_of_event) and
                   (this->Type_of_event == that.Type_of_event) and
                   (this->Number_of_types_for_event_of_model = that.Number_of_types_for_event_of_model) //and
                   //(this->type_of_parent == that.type_of_parent) and
                   //(this->type_of_offspring == that.type_of_offspring)
            ) return true;

              return false;
    }


// functions for changing the Event_of_model
    void set_code_of_event(QString Code_of_event_string);
    void set_name_of_event(QString Name_of_event_in_model);
    void set_type_of_event(QString Type_of_event_in_model);
    void set_number_of_types_for_event_of_model(int Number);
// functions for examining the Model_of_space
    QString get_name_of_event();
    int get_Number_of_types_for_event_of_model();

    // void set_type_of_parent(int parent);
   // void set_type_of_offspring(int offspring);
   // int get_type_of_parent();
   // int get_type_of_offspring();

     QVector<type_of_particle_before_and_after_event> &get_code_of_events();

    //---------------------------------------
};


}//namespace gui
}//namespace simbad


#endif // EVENT_OF_MODEL_H
