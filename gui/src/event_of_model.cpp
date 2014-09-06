#include "event_of_model.h"


simbad::gui::Event_of_model::Event_of_model(int Number_of_types_in_model_for_event)
{
    number_of_types_for_event_of_model=Number_of_types_in_model_for_event;
    Table_of_component_rates A(Number_of_types_in_model_for_event);
    Event_of_model::Table_of_rates=A;

    Distribution_of_particle B;
    Event_of_model::Distribution=B;

    Event_of_model::Code_of_event="--";
    Event_of_model::Name_of_event="--";
    Event_of_model::Type_of_event="--";

}


simbad::gui::Event_of_model::Event_of_model()
{
    number_of_types_for_event_of_model=1;
    Table_of_component_rates A(1);
    Event_of_model::Table_of_rates=A;

    Distribution_of_particle B;
    Event_of_model::Distribution=B;

    Event_of_model::Code_of_event="--";
    Event_of_model::Name_of_event="--";
    Event_of_model::Type_of_event="--";

}



void simbad::gui::Event_of_model::set_code_of_event(QString Code_of_event_in_model)
{
    Code_of_event = Code_of_event_in_model;
}


void simbad::gui::Event_of_model::set_name_of_event(QString Name_of_event_in_model)
{
    Name_of_event = Name_of_event_in_model;
}

void simbad::gui::Event_of_model::set_type_of_event(QString Type_of_event_in_model)
{
    Type_of_event = Type_of_event_in_model;
}


//Event_of_model::Event_of_model(int Number_of_types_in_model)
//{

//}

//Event_of_model::Event_of_model(int Number_of_types_in_model)
//{
//    Table_of_component_rates A(Number_of_types_in_model);
//    Event_of_model.Table_of_rates=A;
//
//}
