#include "event_of_model.h"




simbad::gui::Event_of_model::Event_of_model()
{
    Number_of_types_for_event_of_model = 1;
    Table_of_component_rates_for_event_of_model.Number_of_types = 1;

    Event_of_model::Code_of_event="--";
    Event_of_model::Name_of_event="--";
    Event_of_model::Type_of_event="--";
}



simbad::gui::Event_of_model::Event_of_model(int Number_of_types_in_model_for_event)
{
    Number_of_types_for_event_of_model = Number_of_types_in_model_for_event;
    Table_of_component_rates_for_event_of_model.Number_of_types = Number_of_types_in_model_for_event;

    Event_of_model::Code_of_event="--";
    Event_of_model::Name_of_event="--";
    Event_of_model::Type_of_event="--";
}


simbad::gui::Event_of_model::Event_of_model(const Event_of_model& that)
{
    Name_of_event = that.Name_of_event;
    Code_of_event = that.Code_of_event;
    Type_of_event = that.Type_of_event;
    Number_of_types_for_event_of_model = that.Number_of_types_for_event_of_model;


    Table_of_component_rates_for_event_of_model.Number_of_types = that.Number_of_types_for_event_of_model;

    Table_of_component_rates_for_event_of_model = that.Table_of_component_rates_for_event_of_model;

    Distribution_of_particle_for_event = that.Distribution_of_particle_for_event;


}


simbad::gui::Event_of_model & simbad::gui::Event_of_model::operator = (const Event_of_model& that)
{
    Name_of_event = that.Name_of_event;
    Code_of_event = that.Code_of_event;
    Type_of_event = that.Type_of_event;
    Number_of_types_for_event_of_model = that.Number_of_types_for_event_of_model;

    Table_of_component_rates_for_event_of_model.Number_of_types =
            that.Number_of_types_for_event_of_model;

    Table_of_component_rates_for_event_of_model =
            that.Table_of_component_rates_for_event_of_model;

    Distribution_of_particle_for_event =
            that.Distribution_of_particle_for_event;



    return *this;
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

void simbad::gui::Event_of_model::set_number_of_types_for_event_of_model(int Number)
{
    Number_of_types_for_event_of_model = Number;
}

QString simbad::gui::Event_of_model::get_name_of_event()
{
    return this->Name_of_event;
}


int simbad::gui::Event_of_model::get_Number_of_types_for_event_of_model()
{
    return Number_of_types_for_event_of_model;
}

