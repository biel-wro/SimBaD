#include "model_of_space.h"
#include "event_of_model.h"
#include "iostream"
#include <random>
#include <chrono>
using namespace std;
#define PI 3.14159265
simbad::gui::Model_of_space::Model_of_space()
{
    Name_of_model="";
    dimension_of_model=2;
    number_of_types_in_model=2;
    Global_clock = 0.0;
    Number_of_events_dynamics = 0;
    Number_of_occurings = 0;
}

void simbad::gui::Model_of_space::set_full_file_name(QString File)
{
 FullFileName = File;
}

QString simbad::gui::Model_of_space::get_full_file_name()
{
    return FullFileName;
}

QStringList simbad::gui::Model_of_space::get_configuration_setting_for_file()
{
    QStringList Conf_Q_String_List;

    Conf_Q_String_List.push_back("Number of dimensions: " + QString::number(this->get_number_of_dimentsions())+ "\n");
    Conf_Q_String_List.push_back("Number of point types: " + QString::number(this->get_number_of_types()) + "\n");

    for (int i=0;i<this->get_number_of_types();i++){
        int red=this->ModelPoints.Vector_of_types[i].InitialConfigurationColor.red();
        int green=this->ModelPoints.Vector_of_types[i].InitialConfigurationColor.green();
        int blue=this->ModelPoints.Vector_of_types[i].InitialConfigurationColor.blue();
        Conf_Q_String_List.push_back("Color for initial configuration : \n");
        Conf_Q_String_List.push_back("Red: " + QString::number(red) + "\n");
        Conf_Q_String_List.push_back("Green: " + QString::number(green) + "\n");
        Conf_Q_String_List.push_back("Blue: " + QString::number(blue) + "\n");


        red=this->ModelPoints.Vector_of_types[i].DynamicSimulationColor.red();
        green=this->ModelPoints.Vector_of_types[i].DynamicSimulationColor.green();
        blue=this->ModelPoints.Vector_of_types[i].DynamicSimulationColor.blue();

        Conf_Q_String_List.push_back("Color for dynamical configuration: \n");
        Conf_Q_String_List.push_back("Red: " + QString::number(red) + "\n");
        Conf_Q_String_List.push_back("Green: " + QString::number(green) + "\n");
        Conf_Q_String_List.push_back("Blue: " + QString::number(blue) + "\n");

        Conf_Q_String_List.push_back("Number of points in type "+ QString::number(i+1)+  ": " +
                                     QString::number(this->ModelPoints.
                                                     Vector_of_types[i].
                                                     Number_of_points_in_SpacePointArray)
                                     +"\n");

        for(int j=0;j<this->ModelPoints.Vector_of_types[i].Number_of_points_in_SpacePointArray; j++ )
        {
            Conf_Q_String_List.push_back("BirthTime: " +
                        QString::number(this->ModelPoints.
                                        Vector_of_types[i].
                                        SpacePointArray[j].BirthTime) +
                                         "\n");
            Conf_Q_String_List.push_back("number of point in cell: " +
                        QString::number(this->ModelPoints.
                                        Vector_of_types[i].
                                        SpacePointArray[j].number_of_point_in_cell) +
                                         "\n");

            Conf_Q_String_List.push_back("X coordinate: " +
                        QString::number(this->ModelPoints.
                                        Vector_of_types[i].
                                        SpacePointArray[j].Xcoordinate) +
                                         "\n");

            Conf_Q_String_List.push_back("Y coordinate: " +
                        QString::number(this->ModelPoints.
                                        Vector_of_types[i].
                                        SpacePointArray[j].Ycoordinate) +
                                         "\n");
       }




    };
    return Conf_Q_String_List;

}

QStringList simbad::gui::Model_of_space::get_model_settings_for_file()
{
    QStringList Model_Q_String_List;
    Model_Q_String_List.push_back("Name of model: " + this->get_name_of_model()+ "\n");
    Model_Q_String_List.push_back("Number of dimensions: " + QString::number(this->get_number_of_dimentsions())+ "\n");
    Model_Q_String_List.push_back("Number of point types: " + QString::number(this->get_number_of_types()) + "\n");
    Model_Q_String_List.push_back("Number of events in model: " +QString::number(this->List_of_events_of_model.size()) + "\n");
    Model_Q_String_List.push_back("\n");
   for (int i = 0; i < this->List_of_events_of_model.size(); ++i){
        QString QStr;
        QStr = this->List_of_events_of_model[i].get_name_of_event();
        Model_Q_String_List.push_back(QStr + "\n");

        //Model_Q_String_List.push_back("Code of event :");
        //for (int j = 0; j< this->List_of_events_of_model[i].Code_of_event.size(); j++){

        //    for (int k = 0; k < this->List_of_events_of_model[i].Code_of_event[j].size(); k++){
        //         Model_Q_String_List.push_back(QString::number(this->List_of_events_of_model[i].Code_of_event[j][k]+1));
        //         if (k != (this->List_of_events_of_model[i].Code_of_event[j].size()-1)) Model_Q_String_List.push_back("->");
        //    }
        //    Model_Q_String_List.push_back("\n");

        //    if (j!=(this->List_of_events_of_model[i].Code_of_event.size()-1))
        //        Model_Q_String_List.push_back("               ");
        //};

        for (int j=0; j < this->List_of_events_of_model[i].Table_of_component_rates_for_event_of_model.Table.size(); j++){





            Model_Q_String_List.push_back("influent particle type: " +
                            QString::number(this->List_of_events_of_model[i].
                            Table_of_component_rates_for_event_of_model.
                            Table[j].get_influent_particle_type()) + "\n");

            Functions Func=this->List_of_events_of_model[i].
                    Table_of_component_rates_for_event_of_model.
                                  Table[j].get_Functions();
            QString Str_Func;
            if (Func==null) Str_Func="null";
            if (Func==triangle) Str_Func="triangle";
            if (Func==modul) Str_Func="modul";
            if (Func==one) Str_Func="one";
            if (Func==exp_minus) Str_Func="exp_minus";
            if (Func==zero_next_to_null) Str_Func="zero_next_to_null";
            Model_Q_String_List.push_back("Function: " + Str_Func + "\n" );




            Model_Q_String_List.push_back("Multiplication by constant of function: " +
                            QString::number(this->List_of_events_of_model[i].
                                            Table_of_component_rates_for_event_of_model.
                                            Table[j].get_Multiplication_of_constant()) +"\n");

            Model_Q_String_List.push_back("Restriction of the range: " +
                                          this->List_of_events_of_model[i].
                                          Table_of_component_rates_for_event_of_model.
                                          Table[j].get_string_of_Restrictions() + "\n");

            Model_Q_String_List.push_back("Range of the function: " +
                                          QString::number(this->List_of_events_of_model[i].
                                          Table_of_component_rates_for_event_of_model.
                                          Table[j].get_Range_of_function()) + "\n");

            Approach_of_influence Approach=this->List_of_events_of_model[i].
                    Table_of_component_rates_for_event_of_model.
                                  Table[j].get_Approach_of_influence();
            QString Str_Approach;
            if (Approach==no) Str_Approach="no";
            if (Approach==sum) Str_Approach="sum";
            if (Approach==exp_minus_sum) Str_Approach="exp_minus_sum";
            if (Approach==exp_plus_sum) Str_Approach="exp_plus_sum";
            if (Approach==product) Str_Approach="product";

            Model_Q_String_List.push_back("Approach of the influence: " + Str_Approach + "\n" );


            Model_Q_String_List.push_back("Constant rate plus: " +
                                          QString::number(this->List_of_events_of_model[i].
                                          Table_of_component_rates_for_event_of_model.
                                          Table[j].get_Constant_rate_plus()) + "\n");


        };
        Model_Q_String_List.push_back("Type of new particle: " + QString::number(this->List_of_events_of_model[i].
                                      Distribution_of_particle_for_event.get_Type_of_particle()) + "\n"
                                      );

        Model_Q_String_List.push_back("Type of T student distribution: " + QString::number(this->List_of_events_of_model[i].
                                                      Distribution_of_particle_for_event.get_Type_of_T_student_distribution()) + "\n"
                                      );

        Model_Q_String_List.push_back("Scale paramiter: " + QString::number(this->List_of_events_of_model[i].
                                                      Distribution_of_particle_for_event.get_scale_paramiter()) + "\n"
                                      );

        Model_Q_String_List.push_back("\n");



        //this->List_of_events_of_model.at(i).get_name_of_event()
    };



    //for

    return Model_Q_String_List;

}

bool simbad::gui::Model_of_space::set_configiguration_setting_from_file(QStringList String_list)
{

    int Dimention;
    int Number_of_typ;

    String_list[0].remove("Number of dimensions: ");
    Dimention = String_list[0].toInt();

    String_list[1].remove("Number of point types: ");
    Number_of_typ = String_list[1].toInt();

    if (this->get_number_of_dimentsions()== Dimention &&
            this->get_number_of_types()== Number_of_typ) {

        int current_line_number=1;
        for (int i=0;i<this->get_number_of_types();i++){

            ++current_line_number;

            this->ModelPoints.Number_of_types = this->get_number_of_types();
            String_list[++current_line_number].remove("Red: ");
            this->ModelPoints.Vector_of_types[i].
                    InitialConfigurationColor.setRed(String_list[current_line_number].toInt());
            String_list[++current_line_number].remove("Green: ");
            this->ModelPoints.Vector_of_types[i].
                    InitialConfigurationColor.setGreen(String_list[current_line_number].toInt());
            String_list[++current_line_number].remove("Blue: ");
            this->ModelPoints.Vector_of_types[i].
                    InitialConfigurationColor.setBlue(String_list[current_line_number].toInt());


            ++current_line_number;
            String_list[++current_line_number].remove("Red: ");
            this->ModelPoints.Vector_of_types[i].
                    DynamicSimulationColor.setRed(String_list[current_line_number].toInt());
            String_list[++current_line_number].remove("Green: ");
            this->ModelPoints.Vector_of_types[i].
                    DynamicSimulationColor.setGreen(String_list[current_line_number].toInt());
            String_list[++current_line_number].remove("Blue: ");
            this->ModelPoints.Vector_of_types[i].
                    DynamicSimulationColor.setBlue(String_list[current_line_number].toInt());


            String_list[++current_line_number].remove("Number of points in type " + QString::number(i+1)+  ": ");
            this->ModelPoints.Vector_of_types[i].
                    setNumber_of_points_in_SpacePointArray(
                        String_list[current_line_number].toInt());


            for(int j=0;j<this->ModelPoints.Vector_of_types[i].Number_of_points_in_SpacePointArray; j++ )
            {
                String_list[++current_line_number].remove("BirthTime: ");
                this->ModelPoints.Vector_of_types[i].
                        SpacePointArray[j].BirthTime = String_list[current_line_number].toDouble();

                String_list[++current_line_number].remove("number of point in cell: ");
                this->ModelPoints.Vector_of_types[i].
                        SpacePointArray[j].number_of_point_in_cell = String_list[current_line_number].toInt();


                String_list[++current_line_number].remove("X coordinate: ");
                this->ModelPoints.Vector_of_types[i].
                        SpacePointArray[j].Xcoordinate = String_list[current_line_number].toFloat();

                String_list[++current_line_number].remove("Y coordinate: ");
                this->ModelPoints.Vector_of_types[i].
                        SpacePointArray[j].Ycoordinate = String_list[current_line_number].toFloat();

           };




        };


        return true;
    } else {

        return false;
    };





}


bool simbad::gui::Model_of_space::set_model_setting_for_model_from_file(QStringList String_list)
{
    String_list[0].remove("Name of model: ");
    this->set_name_of_model(String_list[0]);

    String_list[1].remove("Number of dimensions: ");
    this->set_dimension_of_model(String_list[1].toInt());

    String_list[2].remove("Number of point types: ");
    this->set_number_of_types(String_list[2].toInt());

    String_list[3].remove("Number of events in model: ");
    int Nuberm_of_events = String_list[3].toInt();

    int current_line_number=4;
    for (int i=0; i< Nuberm_of_events; i++){


        Event_of_model Current_event(this->get_number_of_types());

        Current_event.set_name_of_event(String_list[++current_line_number]);
        Current_event.set_type_of_event(String_list[current_line_number]);

        Current_event.set_number_of_types_for_event_of_model(this->get_number_of_types());

        Current_event.set_code_of_event(Current_event.get_name_of_event());

        Current_event.Table_of_component_rates_for_event_of_model.Number_of_types = this->get_number_of_types();

        for(int i = 0; i <= this->get_number_of_types() ; i++){
            function_for_component_rate Function_of_com_rate;

            String_list[++current_line_number].remove("influent particle type: ");
            Function_of_com_rate.set_influent_particle_type(String_list[current_line_number].toInt());

            String_list[++current_line_number].remove("Function: ");
            QString Str_Func=String_list[current_line_number];
            Functions Func;
            if (Str_Func=="null") Func=null;
            if (Str_Func=="triangle") Func=triangle;
            if (Str_Func=="modul") Func=modul ;
            if (Str_Func=="one") Func=one ;
            if (Str_Func=="exp_minus") Func=exp_minus;
            if (Str_Func=="zero_next_to_null") Func=zero_next_to_null;
            Function_of_com_rate.set_Function(Func);

            String_list[++current_line_number].remove("Multiplication by constant of function: ");
            Function_of_com_rate.set_Multiplication_by_constant(String_list[current_line_number].toFloat());


            String_list[++current_line_number].remove("Restriction of the range: ");
            Function_of_com_rate.set_Restrictions_from_string(String_list[current_line_number]);

            String_list[++current_line_number].remove("Range of the function: ");
            Function_of_com_rate.set_Range_of_the_function(String_list[current_line_number].toFloat());

            String_list[++current_line_number].remove("Approach of the influence: ");
            QString Str_Approach=String_list[current_line_number];
            Approach_of_influence Appro;
            if (Str_Approach=="no") Appro=no;
            if (Str_Approach=="sum") Appro=sum;
            if (Str_Approach=="exp_minus_sum") Appro=exp_minus_sum;
            if (Str_Approach=="exp_plus_sum") Appro=exp_plus_sum;
            if (Str_Approach=="product") Appro=product;
            Function_of_com_rate.set_Approach_of_influence(Appro);

            String_list[++current_line_number].remove("Constant rate plus: ");
            Function_of_com_rate.set_Constant_rate_plus(String_list[current_line_number].toFloat());
            Current_event.Table_of_component_rates_for_event_of_model.Table.push_back(Function_of_com_rate);
        };

        String_list[++current_line_number].remove("Type of new particle: ");
        Current_event.Distribution_of_particle_for_event.
                set_Type_of_particle(String_list[current_line_number].toInt());

        String_list[++current_line_number].remove("Type of T student distribution: ");
        Current_event.Distribution_of_particle_for_event.
                set_Type_of_Tstudent_distribution(String_list[current_line_number].toInt());

        String_list[++current_line_number].remove("Scale paramiter: ");
        Current_event.Distribution_of_particle_for_event.
                set_scale_paramiter(String_list[current_line_number].toFloat());

        ++current_line_number;

        this->push_back_new_event(Current_event);
    };



//if event was in vector, then we do not do anything
//    QVector<Event_of_model>::const_pointer p = std::find (this->Big_model->List_of_events_of_model.begin(),
//                                                      this->Big_model->List_of_events_of_model.end(), Current_event);;

//    if (p == end(this->Big_model->List_of_events_of_model)) {

//    } else {};




   // Model_Q_String_List.push_back("\n");
 return true;
}

void simbad::gui::Model_of_space::set_dimension_of_model(int Number)
{
    dimension_of_model=Number;
}



int simbad::gui::Model_of_space::get_number_of_dimentsions(){
    return dimension_of_model;
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




bool simbad::gui::Model_of_space::timer()
{


    unsigned seed;
    seed=std::chrono::system_clock::now().time_since_epoch().count()+1;
        mt19937 mtPoissonDistribution(seed);
        double min_for_time=10000;
        int i_min=-1, j_min=-1;

       // cout<<"1"<<endl;

        for (int i = 0; i < this->List_of_events_of_model.size(); i++)
            for (int j = 0; j < this->get_number_of_types() + 1; j++)
            {
                if (Vector_of_Dzeta_type_event[i][j] !=0 ){
                    int Type_of_event_parent_point = this->List_of_events_of_model[i].Code_of_event[0][0];
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// comulative rate depends on general Nubmer of parent points and max(Dzeta rate)_rate of probable neighbors  //
// with corresponding type of influence                                                                       //

                    if (Type_of_event_parent_point != -1) {
                        Vector_comulative_rate_for_type_of_event[i][j] =
                            this->ModelPoints.Vector_of_types[Type_of_event_parent_point]
                            .Number_of_points_in_SpacePointArray *
                            Vector_of_Dzeta_type_event[i][j];
//                                                                                                            //
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    } else {
                        Vector_comulative_rate_for_type_of_event[i][j] =
                            Vector_of_Dzeta_type_event[i][j];

                    };

                    if (Vector_comulative_rate_for_type_of_event[i][j]!=0){
                        exponential_distribution <double> exp_distribution_for_times
                        (Vector_comulative_rate_for_type_of_event[i][j]);
                        Vector_time_before_next_event_of_type[i][j] =
                        exp_distribution_for_times(mtPoissonDistribution);

                    } else {
                        Vector_time_before_next_event_of_type[i][j]= 10001;
                    };

                    if (min_for_time>Vector_time_before_next_event_of_type[i][j])
                    {
                        min_for_time=Vector_time_before_next_event_of_type[i][j];
                        i_min=i;
                        j_min=j;
                    };

                };
            };

        if (i_min != -1 && min_for_time<10000){

            Number_of_events_dynamics = Number_of_events_dynamics + 1;
           // cout<<"Number_of_events_dynamics="<<Number_of_events_dynamics<<endl;
            Min_time = min_for_time;
            Event_execution(i_min, j_min);
            initiale_Dzeta();
            seed=std::chrono::system_clock::now().time_since_epoch().count();
            return true;
        } else return false;

    //};

//      Dzeta_a_minus= 4.0 * (Array_numbers_of_sizes_of_cells.size() - 1) * f_a_minus(0)/5.0;


}

void simbad::gui::Model_of_space::initiate_before_simulation(){
    this->CellMap.setInitConfiguration(this->ModelPoints);

    // times before next events  (type events)
    //reserving memory
    Vector_time_before_next_event_of_type.resize(this->List_of_events_of_model.size());

    for(int i=0; i < this->List_of_events_of_model.size(); i++)
    {
        Vector_time_before_next_event_of_type[i].resize(this->get_number_of_types()+1);
    };


    // comulative rates of type events (type events)
    //reserving memory

    Vector_comulative_rate_for_type_of_event.resize(this->List_of_events_of_model.size());

    for(int i=0; i < this->List_of_events_of_model.size(); i++)
    {
        Vector_comulative_rate_for_type_of_event[i].resize(this->get_number_of_types()+1);
    };




//find max of of_rate_function_radius
    int max_range_of_functions = 0;
    for(int i = 0; i < this->List_of_events_of_model.size(); i++)
    {
        for(int j = 1; j < this->List_of_events_of_model[i].
            Table_of_component_rates_for_event_of_model.Table.size()+1; j++)
        {

            if (max_range_of_functions < this->List_of_events_of_model[i].
                Table_of_component_rates_for_event_of_model.Table[j].get_Range_of_function())
            max_range_of_functions = this->List_of_events_of_model[i].
                  Table_of_component_rates_for_event_of_model.Table[j].get_Range_of_function();
        };
    };
    Proportion_for_radiuses_wrt_Biggest = max_range_of_functions;


    if(max_range_of_functions!=0){
        for(int i = 0; i < this->List_of_events_of_model.size(); i++)
        {
            for(int j = 1; j<this->List_of_events_of_model[i].
                Table_of_component_rates_for_event_of_model.Table.size(); j++)
            {
                float old_range_of_function;
                old_range_of_function = this->List_of_events_of_model[i].
                    Table_of_component_rates_for_event_of_model.Table[j].get_Range_of_function();


                if (max_range_of_functions>old_range_of_function){
                    this->List_of_events_of_model[i].
                        Table_of_component_rates_for_event_of_model.Table[j].set_Range_of_the_function
                        (old_range_of_function/max_range_of_functions *0.05);
                } else {
                    this->List_of_events_of_model[i].
                        Table_of_component_rates_for_event_of_model.Table[j].set_Range_of_the_function
                        (0.05);

                };

            };
        };
    };

    //reserving memory----------------------------------------------------------

    Vector_max_of_rate_functions.resize(this->List_of_events_of_model.size());
    Vector_of_Dzeta_type_event.resize(this->List_of_events_of_model.size());

    for(int i=0; i < this->List_of_events_of_model.size(); i++)
    {
        Vector_max_of_rate_functions[i].resize(this->get_number_of_types()+1);
        Vector_of_Dzeta_type_event[i].resize(this->get_number_of_types()+1);
        //typedef QVector <long double> Dzeta_of_type_event;
        //QVector <Dzeta_of_type_event> ;
    };

    //--------------------------------------------------------------------------

    initiale_Dzeta();

//cout<<"4"<<endl;


    //for (int i=0; i<t+his->List_of_events_of_model.size(); i++){

     //   for (int j=0;j<this->get_number_of_types()+1;j++)
     //   {

    //    };
    //};
//Seed


}

void simbad::gui::Model_of_space::initiale_Dzeta()
{
//    for (int i = 0; i < this->List_of_events_of_model.size(); i++){
//        Vector_max_of_rate_functions[i][0] = this->List_of_events_of_model[i].
//                Table_of_component_rates_for_event_of_model.Table[j].get_max_of_Function();
//        Vector_of_Dzeta_type_event[i][0] = Vector_max_of_rate_functions[i][0];

//        for (int j = 1; j < this->get_number_of_types() + 1; j++)
//        {
// dzeta
//            Vector_of_Dzeta_type_event[i][j] =
//                    4.0*(this->CellMap.Types_Array_numbers_of_sizes_of_cells[
//                    j - 1
                         //this->List_of_events_of_model[i].Code_of_event[0][0]
//                    ].size()-1)*
//                    Vector_max_of_rate_functions[i][j];
//        };
//    };



    for (int i=0; i<this->List_of_events_of_model.size(); i++){

        Vector_max_of_rate_functions[i][0] = this->List_of_events_of_model[i].
                Table_of_component_rates_for_event_of_model.Table[0].get_max_of_Function();
        Vector_of_Dzeta_type_event[i][0]= Vector_max_of_rate_functions[i][0];
//        if (this->List_of_events_of_model[i].Code_of_event[0][0]==-1 &&
//                this->List_of_events_of_model[i].Code_of_event[0][0]!=-1){
//            Vector_of_Dzeta_type_event[i][0]= Vector_max_of_rate_functions[i][0];
//        } else {
//            Vector_of_Dzeta_type_event[i][0] = Vector_max_of_rate_functions[i][0] *
//               this->ModelPoints.Vector_of_types[i].Number_of_points_in_SpacePointArray;
//        };

        for (int j=1;j<this->get_number_of_types() + 1; j++)
        {
//max of functions
            Vector_max_of_rate_functions[i][j] =
                    this->List_of_events_of_model[i].
                    Table_of_component_rates_for_event_of_model.Table[j].get_max_of_Function();
            //cout<<"Vector_max_of_rate_functions["<<i<<"]["<<j<<"]"<<Vector_max_of_rate_functions[i][j] <<endl;

// dzeta
            Vector_of_Dzeta_type_event[i][j] =
                    4.0*(this->CellMap.Types_Array_numbers_of_sizes_of_cells
                    [j-1

                         //this->List_of_events_of_model[i].Code_of_event[0][0]

                    ].size()-1)*
                    Vector_max_of_rate_functions[i][j];
            //cout<<"Vector_of_Dzeta_type_event["<<i<<"]["<<j<<"]"<<Vector_of_Dzeta_type_event[i][j] <<endl;
            //cout<<i<<",*"<<j<<","<<Vector_of_Dzeta_type_event[i][j]<<endl;
        };
    };




}


void simbad::gui::Model_of_space::Event_execution(int Number_of_event, int Number_function_for_component_rate)
{
    //cout<<"yes"<<endl;
    //cout<<"Number_of_event"<<Number_of_event<<endl;
    //cout<<"this->List_of_events_of_model[Number_of_event].Code_of_event.size()="<<
    //       this->List_of_events_of_model[Number_of_event].Code_of_event.size()<<endl;
    int Number_of_changed_points_in_conf =
            this->List_of_events_of_model[Number_of_event].Code_of_event.size();
    //cout<<"Number_of_changed_points_in_conf"<<
    //       Number_of_changed_points_in_conf<<endl;

    if  (Number_of_changed_points_in_conf == 2)
    {
        //birth of particle
        if (this->List_of_events_of_model[Number_of_event].Code_of_event[0][1] != -1)
        {
            //cout << "birth of particle" << endl;
            birth_of_particle(Number_of_event, Number_function_for_component_rate);

        }

        //jump of particle
        if (this->List_of_events_of_model[Number_of_event].Code_of_event[0][1] == -1)
        {
            //cout << "jump of particle" << endl;
            jump_of_particle(Number_of_event, Number_function_for_component_rate);

        }

    };

    if  (Number_of_changed_points_in_conf==1)
    {
        //birth from environment
        if  (this->List_of_events_of_model[Number_of_event].Code_of_event[0][0] == -1)
        {
            //cout<<"birth from environment"<<endl;
            birth_of_particle_from_environment(Number_of_event);

        };

        //mutation of particle
        if (this->List_of_events_of_model[Number_of_event].Code_of_event[0][0]!=
                this->List_of_events_of_model[Number_of_event].Code_of_event[0][1] &&
                    this->List_of_events_of_model[Number_of_event].Code_of_event[0][1]!=-1 &&
                        this->List_of_events_of_model[Number_of_event].Code_of_event[0][0]!=-1
                )
        {
            //cout<<"mutation of particle"<<endl;
            mutation_of_particle(Number_of_event, Number_function_for_component_rate);
        }

        //death of particle
        if (this->List_of_events_of_model[Number_of_event].Code_of_event[0][1]==-1)
        {
            //cout<<"death of particle"<<endl;
            //cout<<"Number_of_event="<<Number_of_event<<" Number_function_for_component_rate="<<Number_function_for_component_rate<<endl;
            death_of_particle(Number_of_event, Number_function_for_component_rate);

        }

    } ;

}


void simbad::gui::Model_of_space::birth_of_particle(int EventNumber, int Numberfunctionforcomponentrate){
    unsigned seed;
    seed=std::chrono::system_clock::now().time_since_epoch().count();
    int number_of_type_of_parent =
            this->List_of_events_of_model[EventNumber].Code_of_event[0][0];

//    cout<<"number_of_type_of_parent"<<number_of_type_of_parent<<endl;

    int number_of_type_of_child = this->List_of_events_of_model[EventNumber].Code_of_event[1][1];
//    cout<<"number_of_type_of_child"<<number_of_type_of_child<<endl;

//    //cout<< "number_of_type_of_death_particle="<<number_of_type_of_death_particle<<endl;
    int Number_of_parent_type_points =
            this->ModelPoints.Vector_of_types[number_of_type_of_parent].
            Number_of_points_in_SpacePointArray;
//    cout<<"Number_of_parent_type_points"<<Number_of_parent_type_points<<endl;
    int Number_of_child_type_points =
            this->ModelPoints.Vector_of_types[number_of_type_of_child].
            Number_of_points_in_SpacePointArray;
    //cout<< "Number_of_points="<<Number_of_points<<endl;
    //cout<<"Number_of_child_type_points"<<Number_of_child_type_points<<endl;

    mt19937 mtUniDistribution(seed);

    uniform_int_distribution<int> distribution_1(0,Number_of_parent_type_points-1);

    int number_of_parent = distribution_1(mtUniDistribution);

    //cout<<"number_of_parent="<<number_of_parent<<endl;





    float rate = rate_linear_on_configuration(EventNumber, Numberfunctionforcomponentrate,
                                              number_of_type_of_parent, number_of_parent);
    //cout<<"rate="<<rate<<endl;
    //cout<<"Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate]"<<
    //Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate]<<endl;
    //bernoulli_distribution distribution_2(rate*0.999/Vector_comulative_rate_for_type_of_event[EventNumber][Numberfunctionforcomponentrate]);
    bernoulli_distribution distribution_2(rate*0.999/Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate]);

    bool bernulli_D = distribution_2(mtUniDistribution);
    //cout<< "bernulli_D="<<bernulli_D<<endl;
    if (bernulli_D){
    //        cout<< "Global_clock="<<Global_clock<<endl;


        float Xcoordinate_of_new_particle;
        float Ycoordinate_of_new_particle;

        if (this->List_of_events_of_model[EventNumber].Distribution_of_particle_for_event.
            get_Type_of_T_student_distribution() == 0){
        //cout<<"qazwsx----+++++"<<endl;
            normal_distribution<float> Normal_distribution(0.0,this->List_of_events_of_model[EventNumber].
                                                           Distribution_of_particle_for_event.get_scale_paramiter());
        //cout<<"scale_paramiter="<<this->List_of_events_of_model[EventNumber].
        //    Distribution_of_particle_for_event.get_scale_paramiter()<<endl;

        //cout<<"befor Xcoordinate_of_new_particle="<<this->ModelPoints.Vector_of_types[number_of_type_of_parent].
        //    SpacePointArray[number_of_parent].Xcoordinate<<endl;
            Xcoordinate_of_new_particle = this->ModelPoints.Vector_of_types[number_of_type_of_parent].
                  SpacePointArray[number_of_parent].Xcoordinate + Normal_distribution(mtUniDistribution);
        //cout<<"after Xcoordinate_of_new_particle="<<Xcoordinate_of_new_particle<<endl;



            Ycoordinate_of_new_particle = this->ModelPoints.Vector_of_types[number_of_type_of_parent].
                    SpacePointArray[number_of_parent].Ycoordinate + Normal_distribution(mtUniDistribution);

        } else {
            uniform_real_distribution<float> uniform_distribution_angle_for_t_student_distribution(0.0,2.0*PI);
            student_t_distribution<float> Student_T_Distribution
                    (this->List_of_events_of_model[EventNumber].Distribution_of_particle_for_event.get_Type_of_T_student_distribution());

            float Student_t_Distibution=this->List_of_events_of_model[EventNumber].
                    Distribution_of_particle_for_event.get_scale_paramiter()
                    *fabs(Student_T_Distribution(mtUniDistribution));
            float angle=uniform_distribution_angle_for_t_student_distribution(mtUniDistribution);

            Xcoordinate_of_new_particle = this->ModelPoints.Vector_of_types[number_of_type_of_parent].
                  SpacePointArray[number_of_parent].Xcoordinate + Student_t_Distibution*cos(angle);


            Ycoordinate_of_new_particle = this->ModelPoints.Vector_of_types[number_of_type_of_parent].
                    SpacePointArray[number_of_parent].Ycoordinate + Student_t_Distibution*
                    sin(angle);

        };

       one_birth(number_of_type_of_child,
                  Xcoordinate_of_new_particle, Ycoordinate_of_new_particle);

        Global_clock = Global_clock + Min_time;

        Number_of_child_type_points=Number_of_child_type_points + 1;
        //cout <<"number_of_type_of_child="<<number_of_type_of_child <<endl;
        this->ModelPoints.Vector_of_types[number_of_type_of_child].
                        SpacePointArray[Number_of_child_type_points - 1].BirthTime = Global_clock;
        //cout <<"Number_of_child_type_points="<<Number_of_child_type_points <<endl;
        //cout <<"ModelPoints.Vector_of_types[number_of_type_of_child].Number_of_points_in_SpacePointArray="<<
        //ModelPoints.Vector_of_types[number_of_type_of_child].Number_of_points_in_SpacePointArray<<endl;
        //cout <<"Global_clock="<<Global_clock<<endl;

        //cout <<"timer::Number_of_child_type_points"<<Number_of_child_type_points<<endl;
        Number_of_occurings = Number_of_occurings + 1;
    };
}


void simbad::gui::Model_of_space::birth_of_particle_from_environment(int EventNumber){
    unsigned seed;
    seed=std::chrono::system_clock::now().time_since_epoch().count();


    int number_of_type_of_child = this->List_of_events_of_model[EventNumber].Code_of_event[0][1];

    int Number_of_child_type_points =
            this->ModelPoints.Vector_of_types[number_of_type_of_child].
            Number_of_points_in_SpacePointArray;
    //cout<<"Number_of_child_type_points="<<Number_of_child_type_points<<endl;

    mt19937 mtUniDistribution(seed);
    float range_of_function=
            this->List_of_events_of_model[EventNumber].
            Table_of_component_rates_for_event_of_model.Table[0].get_Range_of_function();
    uniform_real_distribution<double> uni_distribution(-range_of_function,range_of_function);

    float Xcoordinate_of_new_particle = uni_distribution(mtUniDistribution);
    float Ycoordinate_of_new_particle = uni_distribution(mtUniDistribution);


    one_birth(number_of_type_of_child,
                  Xcoordinate_of_new_particle, Ycoordinate_of_new_particle);

    Global_clock = Global_clock + Min_time;

    Number_of_child_type_points=Number_of_child_type_points + 1;
    //cout <<"number_of_type_of_child="<<number_of_type_of_child <<endl;

    this->ModelPoints.Vector_of_types[number_of_type_of_child].
                        SpacePointArray[Number_of_child_type_points - 1].BirthTime = Global_clock;
    //cout <<"Number_of_child_type_points="<<Number_of_child_type_points <<endl;
    //cout <<"ModelPoints.Vector_of_types[number_of_type_of_child].Number_of_points_in_SpacePointArray="<<
    //ModelPoints.Vector_of_types[number_of_type_of_child].Number_of_points_in_SpacePointArray<<endl;
    //cout <<"Global_clock="<<Global_clock<<endl;

    Number_of_occurings = Number_of_occurings + 1;
}


void simbad::gui::Model_of_space::one_birth(int number_of_type_of_child,
float Xcoordinate_of_new_particle, float Ycoordinate_of_new_particle){

    //--------------
    //cout<<"one_birth::number_of_type_of_child="<<number_of_type_of_child<<endl;

    //cout<<"this->ModelPoints.Vector_of_types[number_of_type_of_child].Number_of_points_in_SpacePointArray="<<
    //      this->ModelPoints.Vector_of_types[number_of_type_of_child].
    //              Number_of_points_in_SpacePointArray<<endl;

    //--------------

    this->ModelPoints.Vector_of_types[number_of_type_of_child].
            Number_of_points_in_SpacePointArray =
    this->ModelPoints.Vector_of_types[number_of_type_of_child].
            Number_of_points_in_SpacePointArray + 1;
    int Number_of_child_type_points = this->ModelPoints.Vector_of_types[number_of_type_of_child].
                    Number_of_points_in_SpacePointArray;
    //cout<<"one_birth::Number_of_child_type_points="<<Number_of_child_type_points<<endl;
    //cout<<"Xcoordinate_of_new_particle="<<Xcoordinate_of_new_particle<<endl;
    //cout<<"Ycoordinate_of_new_particle="<<Ycoordinate_of_new_particle<<endl;
    //cout<<"number_of_type_of_child="<<number_of_type_of_child<<endl;
    //cout<<"this->ModelPoints.Vector_of_types[number_of_type_of_child].SpacePointArray[Number_of_child_type_points-1].Xcoordinate"
    //   <<this->ModelPoints.Vector_of_types[number_of_type_of_child].SpacePointArray[Number_of_child_type_points-1].Xcoordinate<<endl;
    this->ModelPoints.Vector_of_types[number_of_type_of_child].
                    SpacePointArray[Number_of_child_type_points-1].Xcoordinate = Xcoordinate_of_new_particle;
    this->ModelPoints.Vector_of_types[number_of_type_of_child].
                    SpacePointArray[Number_of_child_type_points-1].Ycoordinate = Ycoordinate_of_new_particle;
    //cout<<"q_1"<<endl;
    int addtional_variable=(int) floorf(10 * this->ModelPoints.Vector_of_types[number_of_type_of_child].
                                        SpacePointArray[Number_of_child_type_points-1].Xcoordinate + matrix_scaling_factor * 5) +
        10 * matrix_scaling_factor * floorf(10 * this->ModelPoints.Vector_of_types[number_of_type_of_child].
                                        SpacePointArray[Number_of_child_type_points-1].Ycoordinate + matrix_scaling_factor * 5);
    //cout<<"1"<<endl;
    //добавить индекс элемента в ячейку отвечающую родившейся точке
    this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_child].
            push_back(Number_of_child_type_points - 1);

    //cout<<"2"<<endl;
    //добавить номер элемента в список
    this->ModelPoints.Vector_of_types[number_of_type_of_child].
                    SpacePointArray[Number_of_child_type_points-1].number_of_point_in_cell =
        this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_child].size() - 1;

//cout<<"3"<<endl;


// если ячейка с новой точкой стала содержать максимальное кол-во элементов среди всех ячеек, то


    if (this->CellMap.Array_of_cells[addtional_variable]
            [number_of_type_of_child].size() >
            this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_child].size() - 1){
//уменьшаем кол-во ячеек с предыдущим максимумом (он на ед. меньше чем новый)

       // cout<<"this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_child].size()="<<
       //       this->CellMap.Array_of_cells[addtional_variable]
       //               [number_of_type_of_child].size()<<endl;
         this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_child]
                 [
                     this->CellMap.Array_of_cells[addtional_variable]
                             [number_of_type_of_child].size() - 1
                 ] =
        this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_child]
                 [
                     this->CellMap.Array_of_cells[addtional_variable]
                             [number_of_type_of_child].size() - 1
                 ] - 1;

//cout<<"4"<<endl;
//           Array_numbers_of_sizes_of_cells[Array_numbers_of_sizes_of_cells.size() - 1]=
//              Array_numbers_of_sizes_of_cells[Array_numbers_of_sizes_of_cells.size() - 1] - 1;
//увеличиваем максимум  (тоесть кол-во ячеек с новым максимумом уже стало ровняться 1)
        this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_child].push_back(1);
//            Array_numbers_of_sizes_of_cells.push_back(1);

    } else {
//уменьшаем кол-во ячеек на -1 с размером Array_of_cells_a_mаinus[addtional_variable].size() - 1 ( -1 вычитаем в силу того, что до прибавления
//нового элемента ячейка была именно с этим кол-во элементов)

        this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_child]
                 [
                     this->CellMap.Array_of_cells[addtional_variable]
                            [number_of_type_of_child].size() - 1
                 ] =
        this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_child]
                 [
                      this->CellMap.Array_of_cells[addtional_variable]
                            [number_of_type_of_child].size() - 1
                 ] - 1;
//cout<<"5"<<endl;
      //  Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size() - 1]=
      //      Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size() - 1] - 1;
// увеличиваем кол-во ячеек на +1 с размером Array_of_cells_a_minus[addtional_variable].size() (так-как после повяления нового єлемента
//		в ячейке стано на +1 	больше)
        this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_child]
                 [
                     this->CellMap.Array_of_cells[addtional_variable]
                            [number_of_type_of_child].size()
                 ] =
        this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_child]
                 [
                      this->CellMap.Array_of_cells[addtional_variable]
                            [number_of_type_of_child].size()
                 ] + 1;
//cout<<"6"<<endl;
        //Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size()]=
        //    Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size()] + 1;
    };





//		} else {
//			structure_for_dynamic_changes_of_Dzetas. Number_of_birth_events_without_birth_occuring =
//				structure_for_dynamic_changes_of_Dzetas. Number_of_birth_events_without_birth_occuring + 1;
//			structure_for_dynamic_changes_of_Dzetas. Number_of_birth_occurings_without_birth_impracticable_event = 0;

}


void simbad::gui::Model_of_space::jump_of_particle(int EventNumber, int Numberfunctionforcomponentrate){
    unsigned seed;
    seed=std::chrono::system_clock::now().time_since_epoch().count();

    // cout<< "Yes"<<endl;
    //    cout<< EventNumber<<", "<<Numberfunctionforcomponentrate<<"***"<<endl;
    int number_of_type_of_jump_particle =
            this->List_of_events_of_model[EventNumber].Code_of_event[0][0];
    //  cout<< "number_of_type_of_death_particle="<<number_of_type_of_death_particle<<endl;
    int Number_of_points =
            this->ModelPoints.Vector_of_types[number_of_type_of_jump_particle].
            Number_of_points_in_SpacePointArray;
    //cout<< "Number_of_points="<<Number_of_points<<endl;


    mt19937 mtUniDistribution(seed);
    uniform_int_distribution<int> distribution_1(0,Number_of_points-1);

    int number_of_jump_man = distribution_1(mtUniDistribution);


    if (Numberfunctionforcomponentrate!=0){
        float rate = rate_linear_on_configuration(EventNumber, Numberfunctionforcomponentrate, number_of_type_of_jump_particle, number_of_jump_man);
        bernoulli_distribution distribution_2(rate*0.999/Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate]);




        bool bernulli_D = distribution_2(mtUniDistribution);


        if (bernulli_D){

            float Xcoordinate_of_jump_particle;
            float Ycoordinate_of_jump_particle;

            Xcoordinate_of_jump_particle = this->ModelPoints.Vector_of_types[number_of_type_of_jump_particle].
                    SpacePointArray[number_of_jump_man].Xcoordinate;

            Ycoordinate_of_jump_particle = this->ModelPoints.Vector_of_types[number_of_type_of_jump_particle].
                    SpacePointArray[number_of_jump_man].Ycoordinate;

            float New_Xcoordinate_of_jump_particle;
            float New_Ycoordinate_of_jump_particle;

            if (this->List_of_events_of_model[EventNumber].Distribution_of_particle_for_event.
                    get_Type_of_T_student_distribution() == 0){

                normal_distribution<float> Normal_distribution(0.0,this->List_of_events_of_model[EventNumber].
                                                               Distribution_of_particle_for_event.get_scale_paramiter());


                New_Xcoordinate_of_jump_particle = Xcoordinate_of_jump_particle
                        + Normal_distribution(mtUniDistribution);



                New_Ycoordinate_of_jump_particle = Ycoordinate_of_jump_particle
                        + Normal_distribution(mtUniDistribution);


            } else {
                student_t_distribution<float> Student_T_Distribution
                        (this->List_of_events_of_model[EventNumber].Distribution_of_particle_for_event.get_Type_of_T_student_distribution());

                float Student_t_Distibution=this->List_of_events_of_model[EventNumber].
                        Distribution_of_particle_for_event.get_scale_paramiter()
                        *fabs(Student_T_Distribution(mtUniDistribution));

                uniform_real_distribution<float> uniform_distribution_angle_for_t_student_distribution(0.0,2.0*PI);

                float angle=uniform_distribution_angle_for_t_student_distribution(mtUniDistribution);


                New_Xcoordinate_of_jump_particle = Xcoordinate_of_jump_particle
                        + Student_t_Distibution * cos(angle);



                New_Ycoordinate_of_jump_particle = Ycoordinate_of_jump_particle
                        + Student_t_Distibution *sin(angle);

            };

            one_death(number_of_type_of_jump_particle, number_of_jump_man);
            one_birth(number_of_type_of_jump_particle,
                      New_Xcoordinate_of_jump_particle, New_Ycoordinate_of_jump_particle);                   // number_of_type_of_jump_particle


            Global_clock = Global_clock + Min_time;
            Number_of_occurings = Number_of_occurings + 1;


        };
    } else {
        float Xcoordinate_of_jump_particle;
        float Ycoordinate_of_jump_particle;

        Xcoordinate_of_jump_particle = this->ModelPoints.Vector_of_types[number_of_type_of_jump_particle].
                SpacePointArray[number_of_jump_man].Xcoordinate;

        Ycoordinate_of_jump_particle = this->ModelPoints.Vector_of_types[number_of_type_of_jump_particle].
                SpacePointArray[number_of_jump_man].Ycoordinate;

        float New_Xcoordinate_of_jump_particle;
        float New_Ycoordinate_of_jump_particle;

        if (this->List_of_events_of_model[EventNumber].Distribution_of_particle_for_event.
                get_Type_of_T_student_distribution() == 0){

            normal_distribution<float> Normal_distribution(0.0,this->List_of_events_of_model[EventNumber].
                                                           Distribution_of_particle_for_event.get_scale_paramiter());


            New_Xcoordinate_of_jump_particle = Xcoordinate_of_jump_particle
                    + Normal_distribution(mtUniDistribution);



            New_Ycoordinate_of_jump_particle = Ycoordinate_of_jump_particle
                    + Normal_distribution(mtUniDistribution);


        } else {
            student_t_distribution<float> Student_T_Distribution
                    (this->List_of_events_of_model[EventNumber].Distribution_of_particle_for_event.get_Type_of_T_student_distribution());

            float Student_t_Distibution=this->List_of_events_of_model[EventNumber].
                    Distribution_of_particle_for_event.get_scale_paramiter()
                    *fabs(Student_T_Distribution(mtUniDistribution));

            uniform_real_distribution<float> uniform_distribution_angle_for_t_student_distribution(0.0,2.0*PI);

            float angle=uniform_distribution_angle_for_t_student_distribution(mtUniDistribution);


            New_Xcoordinate_of_jump_particle = Xcoordinate_of_jump_particle
                    + Student_t_Distibution * cos(angle);



            New_Ycoordinate_of_jump_particle = Ycoordinate_of_jump_particle
                    + Student_t_Distibution *sin(angle);

        };

        one_death(number_of_type_of_jump_particle, number_of_jump_man);
        one_birth(number_of_type_of_jump_particle,
                  New_Xcoordinate_of_jump_particle, New_Ycoordinate_of_jump_particle);                   // number_of_type_of_jump_particle


        Global_clock = Global_clock + Min_time;
        Number_of_occurings = Number_of_occurings + 1;

    }


}


void simbad::gui::Model_of_space::mutation_of_particle(int EventNumber, int Numberfunctionforcomponentrate){
    unsigned seed;
    seed=std::chrono::system_clock::now().time_since_epoch().count();

    int number_of_old_type_of_mutated_particle =
            this->List_of_events_of_model[EventNumber].Code_of_event[0][0];
    int number_of_new_type_of_mutated_particle =
            this->List_of_events_of_model[EventNumber].Code_of_event[0][1];
    //  cout<< "number_of_type_of_death_particle="<<number_of_type_of_death_particle<<endl;
    int Number_of_points =
            this->ModelPoints.Vector_of_types[number_of_old_type_of_mutated_particle].
            Number_of_points_in_SpacePointArray;
    //cout<< "Number_of_points="<<Number_of_points<<endl;


    mt19937 mtUniIntDistribution(seed);
    uniform_int_distribution<int> distribution_1(0,Number_of_points-1);

    int number_of_mutated_man = distribution_1(mtUniIntDistribution);

    // cout<< "number_of_dead_man="<<number_of_dead_man<<endl;

    if (Numberfunctionforcomponentrate!=0){
        float rate = rate_linear_on_configuration(EventNumber, Numberfunctionforcomponentrate, number_of_old_type_of_mutated_particle, number_of_mutated_man);
        // cout<< "rate="<<rate<<endl;
        // cout<<"Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate]"<<
        //       Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate];
        //bernoulli_distribution distribution_2(rate*0.999/Vector_comulative_rate_for_type_of_event[EventNumber][Numberfunctionforcomponentrate]);
        bernoulli_distribution distribution_2(rate*0.999/Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate]);

        bool bernulli_D = distribution_2(mtUniIntDistribution);

        if (bernulli_D){
            float Xcoordinate_of_mutated_particle = this->ModelPoints.Vector_of_types[number_of_old_type_of_mutated_particle].
                    SpacePointArray[number_of_mutated_man].Xcoordinate;

            float Ycoordinate_of_mutated_particle = this->ModelPoints.Vector_of_types[number_of_old_type_of_mutated_particle].
                    SpacePointArray[number_of_mutated_man].Ycoordinate;

            //uniform_real_distribution<float> uniform_distribution_angle_for_t_student_distribution(0.0,2.0*3.14);
            //student_t_distribution<float> Student_T_Distribution(0);
            //normal_distribution<float> Normal_distribution(0.0,0.1);


            //float angle_for_student_t_distribution = uniform_distribution_angle_for_t_student_distribution(mtUniIntDistribution);


            //mt19937 mtUniRealDistribution(seed);
            //float New_Xcoordinate_of_jump_particle = Xcoordinate_of_jump_particle
            //        + Normal_distribution(mtUniRealDistribution)*cos(angle_for_student_t_distribution);



            //float New_Ycoordinate_of_jump_particle = Ycoordinate_of_jump_particle + Normal_distribution(mtUniRealDistribution)*
            //        sin(angle_for_student_t_distribution);




            one_death(number_of_old_type_of_mutated_particle, number_of_mutated_man);
            one_birth(number_of_new_type_of_mutated_particle,
                      Xcoordinate_of_mutated_particle, Ycoordinate_of_mutated_particle);                   // number_of_type_of_jump_particle





            Global_clock = Global_clock + Min_time;
            //if (this->ModelPoints.Vector_of_types[number_of_new_type_of_mutated_particle].SpacePointArray.size() != 0)
            //{
                //this->ModelPoints.Vector_of_types[number_of_new_type_of_mutated_particle].
                //            SpacePointArray
                //[
                //
                //    this->ModelPoints.Vector_of_types[number_of_new_type_of_mutated_particle].
                //        Number_of_points_in_SpacePointArray - 1
                //
                //
                //].BirthTime = Global_clock;

            //};
            Number_of_occurings = Number_of_occurings + 1;

        };

    } else {
        float Xcoordinate_of_mutated_particle = this->ModelPoints.Vector_of_types[number_of_old_type_of_mutated_particle].
                SpacePointArray[number_of_mutated_man].Xcoordinate;

        float Ycoordinate_of_mutated_particle = this->ModelPoints.Vector_of_types[number_of_old_type_of_mutated_particle].
                SpacePointArray[number_of_mutated_man].Ycoordinate;

        //uniform_real_distribution<float> uniform_distribution_angle_for_t_student_distribution(0.0,2.0*3.14);
        //student_t_distribution<float> Student_T_Distribution(0);
        //normal_distribution<float> Normal_distribution(0.0,0.1);


        //float angle_for_student_t_distribution = uniform_distribution_angle_for_t_student_distribution(mtUniIntDistribution);


        //mt19937 mtUniRealDistribution(seed);
        //float New_Xcoordinate_of_jump_particle = Xcoordinate_of_jump_particle
        //        + Normal_distribution(mtUniRealDistribution)*cos(angle_for_student_t_distribution);



        //float New_Ycoordinate_of_jump_particle = Ycoordinate_of_jump_particle + Normal_distribution(mtUniRealDistribution)*
        //        sin(angle_for_student_t_distribution);




        one_death(number_of_old_type_of_mutated_particle, number_of_mutated_man);
        one_birth(number_of_new_type_of_mutated_particle,
                  Xcoordinate_of_mutated_particle, Ycoordinate_of_mutated_particle);                   // number_of_type_of_jump_particle





        Global_clock = Global_clock + Min_time;
         //if (this->ModelPoints.Vector_of_types[number_of_new_type_of_mutated_particle].SpacePointArray.size() != 0)
         //{
         //  this->ModelPoints.Vector_of_types[number_of_new_type_of_mutated_particle].
         //               SpacePointArray
         //   [

         //       this->ModelPoints.Vector_of_types[number_of_new_type_of_mutated_particle].
         //           Number_of_points_in_SpacePointArray - 1


         //   ].BirthTime = Global_clock;
         //
        //};
        Number_of_occurings = Number_of_occurings + 1;

    };




}


void simbad::gui::Model_of_space::death_of_particle(int EventNumber, int Numberfunctionforcomponentrate){
    unsigned seed;
    seed=std::chrono::system_clock::now().time_since_epoch().count();

   // cout<< "Yes"<<endl;
//    cout<< EventNumber<<", "<<Numberfunctionforcomponentrate<<"***"<<endl;
    int number_of_type_of_death_particle =
            this->List_of_events_of_model[EventNumber].Code_of_event[0][0];
  //  cout<< "number_of_type_of_death_particle="<<number_of_type_of_death_particle<<endl;
    int Number_of_points =
            this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
            Number_of_points_in_SpacePointArray;
    //cout<< "Number_of_points="<<Number_of_points<<endl;


    mt19937 mtUniIntDistribution(seed);
    uniform_int_distribution<int> distribution_1(0,Number_of_points-1);

    int number_of_dead_man = distribution_1(mtUniIntDistribution);
   // cout<< "number_of_dead_man="<<number_of_dead_man<<endl;

    if (Numberfunctionforcomponentrate!=0){
        float rate = rate_linear_on_configuration(EventNumber, Numberfunctionforcomponentrate, number_of_type_of_death_particle, number_of_dead_man);
   // cout<< "rate="<<rate<<endl;
   // cout<<"Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate]"<<
   //       Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate];
    //bernoulli_distribution distribution_2(rate*0.999/Vector_comulative_rate_for_type_of_event[EventNumber][Numberfunctionforcomponentrate]);
        bernoulli_distribution distribution_2(rate*0.999/Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate]);

        bool bernulli_D = distribution_2(mtUniIntDistribution);
    //cout<< "bernulli_D="<<bernulli_D<<endl;
        if (bernulli_D){
            one_death(number_of_type_of_death_particle, number_of_dead_man);
            Global_clock = Global_clock + Min_time;
            Number_of_occurings = Number_of_occurings + 1;

        };
    } else {
        one_death(number_of_type_of_death_particle, number_of_dead_man);
        Global_clock = Global_clock + Min_time;
        Number_of_occurings = Number_of_occurings + 1;

    }


}



void simbad::gui::Model_of_space::one_death(int number_of_type_of_death_particle, int number_of_dead_man){
    int Number_of_points =
            this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
            Number_of_points_in_SpacePointArray;
    // информация о будущей мертвой точки =  координаты + номер который имеет точка в ячейке
            float X_coord_of_dad_man = this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                    SpacePointArray[number_of_dead_man].Xcoordinate;
            float Y_coord_of_dad_man = this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                    SpacePointArray[number_of_dead_man].Ycoordinate;

            int Cell_num_of_dad_man = this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                    SpacePointArray[number_of_dead_man]. number_of_point_in_cell;
    //
    // addtional_variable - номер ячейки в которую входит будущая мертвая точка с номером number_of_dead_man
            int addtional_variable=(int) floorf(10 * X_coord_of_dad_man + matrix_scaling_factor * 5)
                + 10 * matrix_scaling_factor * floorf(10 * Y_coord_of_dad_man + matrix_scaling_factor * 5);



    //			structure_for_dynamic_changes_of_Dzetas.Number_of_death_events_without_death_occuring=0;
    //			structure_for_dynamic_changes_of_Dzetas.Number_of_death_occurings_without_death_impracticable_event=structure_for_dynamic_changes_of_Dzetas.Number_of_death_occurings_without_death_impracticable_event+1;


    //В этом блоке работаем с ситуацией, когда выбраная точка для смерти number_of_dead_man не крайняя тоесть не с номером = Number_of_points - 1
            if (Number_of_points - 1 != number_of_dead_man){



    // ///////////////////////////////////////////////////////////////
    //Этот блок относиться к изменению массива отвечающего за Dzeta//
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!///////////
    //если кол-во ячеек с размером (кол-вом точек в ячейке) Array_of_cells_a_minus[addtional_variable].size() больше одной, то
    // уменьшаем кол-во ячеек с размером Array_of_cells_a_minus[addtional_variable].size() на 1;

    //					try { *.at
    //					} catch (const std::out_of_range& oor) {
    //						cerr << "size of Array_of_cells_a_minus[addtional_variable] "<< Array_of_cells_a_minus[addtional_variable].size() << '\n';
    //			};


                this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                        [
                            this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_death_particle].size()
                        ]
                        =
                this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                        [
                            this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_death_particle].size()
                        ] - 1;

                //Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size()]=
                //    Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size()] - 1;

                if ( (this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                      [
                          this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_death_particle].size()
                      ] == 0) &&
                        (this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_death_particle].size()
                         == this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                         .size()-1)
                    ){
                    this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle].pop_back();
                };



    // и увеличивам кол-во ячеек с размером Array_of_cells_a_minus[addtional_variable].size()-1 на 1;
    //				if (Array_of_cells_a_minus[addtional_variable].size()>0){

                this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                        [
                          this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_death_particle].size()-1
                        ]
                        =
                this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                        [
                          this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_death_particle].size()-1

                        ] + 1;


    //                  Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size() - 1]=
    //                   Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size() - 1] + 1;

    //				};
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
    // ///////////////////////////////////////////////////////////////

    // ///////////////////////////////////////////////////////////////
    //															  //

                int The_number_of_last_element_in_cell_with_number_addtional_variable =
                        this->CellMap.Array_of_cells[addtional_variable][number_of_type_of_death_particle].size() - 1;
                        //Array_of_cells_a_minus[addtional_variable].size()-1;

    //В этой переменной мы сохраняем номер элемента главного массива точек на который указывает крайний элемент ячейки addtional_variable
    // то-есть ячейки в которой находиться будущий мертвый элемент
                int Number_of_point_in_array_of_points =
                        CellMap.Array_of_cells[addtional_variable]
                        [number_of_type_of_death_particle]
                        [The_number_of_last_element_in_cell_with_number_addtional_variable];

                        //Array_of_cells_a_minus[addtional_variable]
                        //[The_number_of_last_element_in_cell_with_number_addtional_variable];

    // сохраняем в этму переменную номер точки (будужей мертвой точки) в ячейке

                int Number_of_point_of_dead_man_in_cell =
                        this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].SpacePointArray
                        [number_of_dead_man].number_of_point_in_cell;
                        //Array_of_points[number_of_dead_man].number_of_point_in_cell;

    //тут мы меняем  номер number_of_point_in_cell элемента главного массива точек на который указывает крайний элемент ячейки
    //на номер точки в ячейки будуще мертвой точки

                this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].SpacePointArray
                        [Number_of_point_in_array_of_points].number_of_point_in_cell =
                        Number_of_point_of_dead_man_in_cell;
                //        Array_of_points[Number_of_point_in_array_of_points].number_of_point_in_cell =
                //        Number_of_point_of_dead_man_in_cell;

    //меняем номер (мертвой точки в ячейке соответствующей этой самой точке) указывающий на элемент главного массива

                this->CellMap.Array_of_cells[addtional_variable]
                        [number_of_type_of_death_particle]
                        [Number_of_point_of_dead_man_in_cell]= Number_of_point_in_array_of_points;
                //Array_of_cells_a_minus[addtional_variable][Number_of_point_of_dead_man_in_cell] =
                //Number_of_point_in_array_of_points;

    // удаляем последний элемент в ячейке (мертвой точки)
                this->CellMap.Array_of_cells[addtional_variable]
                        [number_of_type_of_death_particle].pop_back();

                //Array_of_cells_a_minus[addtional_variable].pop_back();
    //                                                                     //
    // //////////////////////////////////////////////////////////////////////



    // сейчас работаем с точкой с крайним номером
                float X_coord_last_point = this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray[Number_of_points-1].Xcoordinate;

                        //Array_of_points[Number_of_points-1]. Xcoordinate;
                float Y_coord_last_point = this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray[Number_of_points-1].Ycoordinate;
                        //Array_of_points[Number_of_points-1]. Ycoordinate;
                int Cell_num_last_point = this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray[Number_of_points-1].number_of_point_in_cell;
                        //Array_of_points[Number_of_points-1]. number_of_point_in_cell;
//                long double Birth_time_last_point = this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
//                        SpacePointArray[Number_of_points-1].BirthTime;


    // номер ячейки с точкой с крайним номером
                int addtional_variable_last_element=(int) floorf(10 * X_coord_last_point + matrix_scaling_factor * 5)
                + 10 * matrix_scaling_factor*floorf(10 * Y_coord_last_point + matrix_scaling_factor*5);




                this->CellMap.Array_of_cells[addtional_variable_last_element][
                        number_of_type_of_death_particle]
                 [Cell_num_last_point] = number_of_dead_man;
                //Array_of_cells_a_minus[addtional_variable_last_element][Cell_num_last_point] = number_of_dead_man;


    // копируем всю информацию о последней точке в будущую мертвую точку
                this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray[number_of_dead_man].Xcoordinate =
                        this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray [Number_of_points-1].Xcoordinate;

                //Array_of_points[number_of_dead_man].Xcoordinate = Array_of_points[Number_of_points-1].Xcoordinate;
                this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray[number_of_dead_man].Ycoordinate =
                        this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray [Number_of_points-1].Ycoordinate;


                //Array_of_points[number_of_dead_man].Ycoordinate = Array_of_points[Number_of_points-1].Ycoordinate;
                this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray[number_of_dead_man].number_of_point_in_cell =
                        this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray [Number_of_points-1].number_of_point_in_cell;

                this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray[number_of_dead_man].BirthTime =
                        this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                        SpacePointArray [Number_of_points-1].BirthTime;


                //Array_of_points[number_of_dead_man].number_of_point_in_cell = Array_of_points[Number_of_points-1].number_of_point_in_cell;


    //В этом блоке работаем с ситуацией, когда выбраная точка для смерти number_of_dead_man крайняя тоесть с номером = Number_of_points - 1
            } else {





    // ///////////////////////////////////////////////////////////////
    //Этот блок относиться к изменению массива отвечающего за Dzeta//
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!///////////
    //если кол-во ячеек с размером (кол-вом точек в ячейке) Array_of_cells_a_minus[addtional_variable].size() больше одной, то
    // уменьшаем кол-во ячеек с размером Array_of_cells_a_minus[addtional_variable].size() на 1;


    //					try { *.at
    //					} catch (const std::out_of_range& oor) {
    //						cerr << "size of Array_of_cells_a_minus[addtional_variable] "<< Array_of_cells_a_minus[addtional_variable].size() << '\n';
    //			};

    // уменьшаем на 1 кол-во ячеек с размеров Array_of_cells_a_minus[addtional_variable].size() так, как
    // будет уменьшено кол-во элементов в ячейке Array_of_cells_a_minus[addtional_variable]

               this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                        [
                            this->CellMap.Array_of_cells[addtional_variable]
                                    [number_of_type_of_death_particle].size()
                        ] =
               this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                        [
                            this->CellMap.Array_of_cells[addtional_variable]
                                    [number_of_type_of_death_particle].size()
                        ] - 1;


                //Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size()]=
                   // Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size()] - 1;
    // если после удаления кол-во ячеек с размером Array_of_cells_a_minus[addtional_variable].size()
    // стало 0 и максимальное кол-во элементов во всех ячейках совпадает с размером Array_of_cells_a_minus[addtional_variable].size()

                if ( (           this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                                 [
                                     this->CellMap.Array_of_cells[addtional_variable]
                                             [number_of_type_of_death_particle].size()
                                 ] == 0) &&
                        (this->CellMap.Array_of_cells[addtional_variable]
                         [number_of_type_of_death_particle].size() ==
                         this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle].
                         size()-1)
                    ){
    // то просто уменьшаем размер максимума по всем ячейкам на 1 (больше не нужно так-как появилась ячейка с размером на ед. меньше чем пред макс по размерам ячеек)
                        this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                            .pop_back();
                };



    // и увеличивам кол-во ячеек с размером Array_of_cells_a_minus[addtional_variable].size()-1 на 1;
    // проблем с -1 как индекса массива нет, так - как размер ячейки как минимум 1 ( одна в будущем мертвая точка там имеется )

                this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                        [
                            this->CellMap.Array_of_cells[addtional_variable]
                                [number_of_type_of_death_particle].size() -1
                        ]=
                this->CellMap.Types_Array_numbers_of_sizes_of_cells[number_of_type_of_death_particle]
                        [
                                    this->CellMap.Array_of_cells[addtional_variable]
                                        [number_of_type_of_death_particle].size() -1
                        ] +1;
                //Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size()-1]=
                    //Array_numbers_of_sizes_of_cells[Array_of_cells_a_minus[addtional_variable].size()-1] + 1;


    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
    // ///////////////////////////////////////////////////////////////
    // если будущая мертвая точка посылаеться на поседний элемент соответствующей ячейки
    // то
                if (Cell_num_of_dad_man == this->CellMap.Array_of_cells[addtional_variable]
                        [number_of_type_of_death_particle].size() - 1) {
                //if (Cell_num_of_dad_man == Array_of_cells_a_minus[addtional_variable].size()-1) {

    // удаляем последний элемент ячейки
                    this->CellMap.Array_of_cells[addtional_variable]
                                        [number_of_type_of_death_particle].pop_back();
                    //Array_of_cells_a_minus[addtional_variable].pop_back();
                } else {
    // если в будущем мертвая точка не посылаеться на поседний элемент ячеки то
    // записываем в M индекс главного массивая точек, на который ссылаеться крайний элемент ячейки соответствующей в будущем мертвой точке

                    int M = this->CellMap.Array_of_cells[addtional_variable]
                            [number_of_type_of_death_particle]
                            [
                                this->CellMap.Array_of_cells[addtional_variable]
                                 [number_of_type_of_death_particle].size() - 1
                            ];
                    //int M = Array_of_cells_a_minus[addtional_variable]
                    //[Array_of_cells_a_minus[addtional_variable].size()-1];

    // переписываем  посылание элемента (который соответствует в будущем мертвой точке) ячейки (соотве. в будущем мертвой точке)
    //					на индекс главного массивая точек

                    this->CellMap.Array_of_cells[addtional_variable]
                                                [number_of_type_of_death_particle]
                                                [Cell_num_of_dad_man] = M;

                    //Array_of_cells_a_minus[addtional_variable][Cell_num_of_dad_man] = M;
    // перенаправляем соответствующее посылание в главном массиве точек

                    this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                            SpacePointArray[M].number_of_point_in_cell = Cell_num_of_dad_man;
                    //Array_of_points[M].number_of_point_in_cell= Cell_num_of_dad_man;

    // удаляем последний элемент ячейки
                    this->CellMap.Array_of_cells[addtional_variable]
                            [number_of_type_of_death_particle].pop_back();
                    //Array_of_cells_a_minus[addtional_variable].pop_back();

                };

            };


            this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                    Number_of_points_in_SpacePointArray =
            this->ModelPoints.Vector_of_types[number_of_type_of_death_particle].
                    Number_of_points_in_SpacePointArray - 1;
                    Number_of_points = Number_of_points - 1;


                    //for (int k=0; k<this->ModelPoints.Number_of_types; k++)
                    //    for (int i=0;i<this->CellMap.Types_Array_numbers_of_sizes_of_cells[k].size() ;i++)
                    //        cout <<"Types_Array_numbers_of_sizes_of_cells[k][i]="<<
                    //               this->CellMap.Types_Array_numbers_of_sizes_of_cells[k][i]<<endl;



}



float simbad::gui::Model_of_space::rate_linear_on_configuration(int EventNumber, int Numberfunctionforcomponentrate,int Number_of_type_of_particle, int Number_of_point_in_type_array)
{
    //сумма всех
    float sum = 0.0;


    float Xcoord,Ycoord;
    Xcoord = this->ModelPoints.Vector_of_types[Number_of_type_of_particle].SpacePointArray[Number_of_point_in_type_array].Xcoordinate;
    Ycoord = this->ModelPoints.Vector_of_types[Number_of_type_of_particle].SpacePointArray[Number_of_point_in_type_array].Ycoordinate;
    int X_number = (int) floorf(10* Xcoord
                              + matrix_scaling_factor*5);
    int Y_number = (int) floorf(10 * Ycoord
            + matrix_scaling_factor*5);

    int addtional_variable = X_number + 10 * matrix_scaling_factor * Y_number;
    //unsigned int Left_border_of_range= X_number

    //для выбора 4 ячеек
    int X_indicator,
        Y_indicator;

    if (10 * Xcoord + matrix_scaling_factor*5-X_number-0.5 <= 0) {X_indicator=-1;
    } else  X_indicator=1;


    if (10 * Ycoord + matrix_scaling_factor*5-Y_number-0.5<=0) {Y_indicator=-1;
    } else  Y_indicator=1;

    int addtional_variable_1 = addtional_variable + X_indicator;
    int addtional_variable_2 = addtional_variable + 10 * matrix_scaling_factor * Y_indicator;
    int	addtional_variable_3 = addtional_variable + 10 * matrix_scaling_factor * Y_indicator + X_indicator;

    int Addtional_vailable[4];

    Addtional_vailable[0]=addtional_variable;
    Addtional_vailable[1]=addtional_variable_1;
    Addtional_vailable[2]=addtional_variable_2;
    Addtional_vailable[3]=addtional_variable_3;

    //int type_of_infuence_points = this->



    for(int j = 0; (j < 4); j++){
        for(int i=0; i < this->CellMap.Array_of_cells[Addtional_vailable[j]]
            [Numberfunctionforcomponentrate-1].size(); i++){
            float argument_of_function_a;
            float function_a;

            float difference_X	=
                    this->ModelPoints.Vector_of_types[Numberfunctionforcomponentrate-1].SpacePointArray
                    [
                        this->CellMap.Array_of_cells[Addtional_vailable[j]]
                        [Numberfunctionforcomponentrate-1][i]
                    ].Xcoordinate
                -   Xcoord;

            float difference_Y =
                   this->ModelPoints.Vector_of_types[Numberfunctionforcomponentrate-1].SpacePointArray
                   [
                       this->CellMap.Array_of_cells[Addtional_vailable[j]]
                       [Numberfunctionforcomponentrate-1][i]
                   ].Ycoordinate
               -   Ycoord;
            float Radius_of_a_minus = this->List_of_events_of_model[EventNumber].
                   Table_of_component_rates_for_event_of_model.Table[Numberfunctionforcomponentrate].
                   get_Range_of_function();

            if ( difference_X * difference_X +
                  difference_Y * difference_Y <=
                    Radius_of_a_minus*Radius_of_a_minus)
            {
                argument_of_function_a = pow(
                            pow(difference_X,2.0) + pow(difference_Y,2.0),
                            0.5);

                function_a = this->List_of_events_of_model[EventNumber].
                        Table_of_component_rates_for_event_of_model.Table[Numberfunctionforcomponentrate].
                        get_value_of_Function(argument_of_function_a);

                sum = sum + function_a;
                //Number_of_neighbours  = Number_of_neighbours + 1;
            };

        };
    };



    if (Numberfunctionforcomponentrate - 1 == Number_of_type_of_particle)
        sum = sum - this->List_of_events_of_model[EventNumber].
            Table_of_component_rates_for_event_of_model.Table[Numberfunctionforcomponentrate].
            get_value_of_Function(0);

    //if (sum > Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate])
    //    sum = Vector_of_Dzeta_type_event[EventNumber][Numberfunctionforcomponentrate];

    return sum;



}


long double simbad::gui::Model_of_space::get_global_clock()
{
    return Global_clock;
}
