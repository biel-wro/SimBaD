#include "event_of_model.h"
#include "iostream"
using namespace std;

simbad::gui::Event_of_model::Event_of_model()
{
    Number_of_types_for_event_of_model = 1;
    Table_of_component_rates_for_event_of_model.Number_of_types = 1;



   // Event_of_model::Code_of_event="--";
    Event_of_model::Name_of_event="--";
    Event_of_model::Type_of_event="--";
}



simbad::gui::Event_of_model::Event_of_model(int Number_of_types_in_model_for_event)
{
    Number_of_types_for_event_of_model = Number_of_types_in_model_for_event;
    Table_of_component_rates_for_event_of_model.Number_of_types = Number_of_types_in_model_for_event;

    //Event_of_model::Code_of_event="--";
    Event_of_model::Name_of_event="--";
    Event_of_model::Type_of_event="--";


}


simbad::gui::Event_of_model::Event_of_model(const Event_of_model& that)
{
    Name_of_event = that.Name_of_event;
    //Code_of_event = that.Code_of_event;
    Type_of_event = that.Type_of_event;
    Number_of_types_for_event_of_model = that.Number_of_types_for_event_of_model;


    Table_of_component_rates_for_event_of_model.Number_of_types = that.Number_of_types_for_event_of_model;

    Table_of_component_rates_for_event_of_model = that.Table_of_component_rates_for_event_of_model;

    Distribution_of_particle_for_event = that.Distribution_of_particle_for_event;


    Code_of_event.resize(that.Code_of_event.size());

    for (int i=0;i<that.Code_of_event.size();i++)
    {
        Code_of_event[i].resize(that.Code_of_event[i].size());
    };

    for (int i=0; i<that.Code_of_event.size();i++)
    {
        for(int j=0; j<that.Code_of_event[i].size();j++)
        {
            Code_of_event[i][j] = that.Code_of_event[i][j];
        }
    };





}


simbad::gui::Event_of_model & simbad::gui::Event_of_model::operator = (const Event_of_model& that)
{
    Name_of_event = that.Name_of_event;
   // Code_of_event = that.Code_of_event;
    Type_of_event = that.Type_of_event;
    Number_of_types_for_event_of_model = that.Number_of_types_for_event_of_model;

    Table_of_component_rates_for_event_of_model.Number_of_types =
            that.Number_of_types_for_event_of_model;

    Table_of_component_rates_for_event_of_model =
            that.Table_of_component_rates_for_event_of_model;

    Distribution_of_particle_for_event =
            that.Distribution_of_particle_for_event;

    Code_of_event.resize(that.Code_of_event.size());
    for (int i=0;i<that.Code_of_event.size();i++)
    {
        Code_of_event[i].resize(that.Code_of_event[i].size());
    };

    for (int i=0; i<that.Code_of_event.size();i++)
    {
        for(int j=0; j<that.Code_of_event[i].size();j++)
        {
            Code_of_event[i][j] = that.Code_of_event[i][j];
        }
    };

    return *this;
}





//void simbad::gui::Event_of_model::set_code_of_event(QVector <type_of_particle_before_and_after_event> & Code_of_event_in_model)
//{
//    Code_of_event.resize(Code_of_event_in_model.size());
//    for (int i=0;i<Code_of_event_in_model.size();i++)
//    {
//        Code_of_event[i].resize(Code_of_event_in_model[i].size());
//    };

//    for (int i=0; i<Code_of_event_in_model.size();i++)
//    {
//        for(int j=0; j<Code_of_event_in_model[i].size();j++)
//        {
//            Code_of_event[i][j] = Code_of_event_in_model[i][j];
//        }
//    };

//}


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
//void set_code_of_event(QString Code_of_event_string);


void simbad::gui::Event_of_model::set_code_of_event(QString Code_of_event_string)
{

    if (Code_of_event_string.contains("birth of one particle with type # ")) {
        int New_point_type ;
        int Old_point_type;
        Code_of_event_string.remove("birth of one particle with type # ");

        if (Code_of_event_string.contains(" from an environment"))
        {
            Code_of_event_string.remove(" from an environment");
            int Number = Code_of_event_string.toInt();
            New_point_type = Number;
            Code_of_event.resize(1);
            Code_of_event[0].resize(2);
            Code_of_event[0][0] = - 1;
            Code_of_event[0][1] = New_point_type - 1;

            //cout << Number << endl;
            //cout << New_point_type << endl;


        } else {
            Code_of_event_string.remove(" from particle with type # ");

            //int Number = Code_of_event_string.toInt();
            int Number = Code_of_event_string.toInt();
            New_point_type = (int)((Number*10/100));
            Old_point_type = Number - 10 * New_point_type;
            Code_of_event.resize(2);
            Code_of_event[0].resize(2);
            Code_of_event[1].resize(2);
            Code_of_event[0][0] = Old_point_type-1;
            Code_of_event[0][1] = Old_point_type-1;
            Code_of_event[1][0] = -1;
            Code_of_event[1][1] = New_point_type-1;

            //cout << Number << endl;
            //cout << Old_point_type << endl;
            //cout << New_point_type << endl;

        };

            //" , " <<Old_point_type <<" , " <<New_point_type<<
            // std::cout << "void simbad::gui::Dialog_open_model::on_pushButton_4_clicked()++++" << std::endl;



    } ;

    if (Code_of_event_string.contains("death of one particle with type # ")){

        Code_of_event_string.remove("death of one particle with type # ");
        int Old_point_type= Code_of_event_string.toInt();

        //cout << Old_point_type << endl;


        Code_of_event.resize(1);
        Code_of_event[0].resize(2);


        Code_of_event[0][0] = Old_point_type-1;
        Code_of_event[0][1] = -1;

    };

    if (Code_of_event_string.contains("jump of one particle with type # ")){

        Code_of_event_string.remove("jump of one particle with type # ");
        int point_type = Code_of_event_string.toInt();

        //cout << point_type << endl;

        Code_of_event.resize(2);
        Code_of_event[0].resize(2);
        Code_of_event[1].resize(2);


        Code_of_event[0][0] = point_type-1;
        Code_of_event[0][1] = -1;

        Code_of_event[1][0] = -1;
        Code_of_event[1][1] = point_type-1;

    };


    if (Code_of_event_string.contains("mutation of one particle with type # ")){

        Code_of_event_string.remove("mutation of one particle with type # ");
        Code_of_event_string.remove(" to particle with type # ");
        //int Number = Code_of_event_string.toInt();
        int Number = Code_of_event_string.toInt();
        int Old_point_type = (int)((Number*10/100));
        int New_point_type = Number - 10*Old_point_type;



        Code_of_event.resize(1);
        Code_of_event[0].resize(2);


        Code_of_event[0][0] = Old_point_type-1;
        Code_of_event[0][1] = New_point_type-1;

       // cout << Number << endl;
       // cout << Old_point_type << endl;
       // cout << New_point_type << endl;


    };



}



QVector <simbad::gui::Event_of_model::type_of_particle_before_and_after_event>&
simbad::gui::Event_of_model::get_code_of_events()
{
    return Code_of_event;
}


//void simbad::gui::Event_of_model::set_type_of_parent(int parent)
//{
//    type_of_parent=parent;
//}
//void simbad::gui::Event_of_model::set_type_of_offspring(int offspring)
//{
//    type_of_offspring=offspring;
//}
//int simbad::gui::Event_of_model::get_type_of_parent()
//{
//    return type_of_parent;
//}
//int simbad::gui::Event_of_model::get_type_of_offspring()
//{
//    return type_of_offspring;
//}


