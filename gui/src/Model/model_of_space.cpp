#include "model_of_space.h"
#include "event_of_model.h"


simbad::gui::Model_of_space::Model_of_space()
{
    Name_of_model="";
    dimention_of_model=2;
    number_of_types_in_model=2;
}


simbad::gui::Model_of_space::~Model_of_space()
{

}


void simbad::gui::Model_of_space::set_name_of_model(QString Name_of_the_model)
{
    Name_of_model = Name_of_the_model;
}

void simbad::gui::Model_of_space::set_number_of_types(int Number)
{
    number_of_types_in_model = Number;
}


QString simbad::gui::Model_of_space::get_name_of_model()
{
   return Name_of_model;
}

int simbad::gui::Model_of_space::get_number_of_types()
{

   return number_of_types_in_model;
}

void simbad::gui::Model_of_space::push_back_new_event(const Event_of_model &New_event)
{
    this->List_of_events_of_model.push_back(New_event);
}


QVector <simbad::gui::Event_of_model> simbad::gui::Model_of_space::get_vector_of_events()
{
    return this->List_of_events_of_model;
}
