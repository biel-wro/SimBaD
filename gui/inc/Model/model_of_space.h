#ifndef MODEL_OF_SPACE_H
#define MODEL_OF_SPACE_H
#include <QtCore>
#include "event_of_model.h"
#include "points_of_model.h"
#include "cell_map.h"
#include <random>
#include <chrono>

//namespace Ui {
//    class Model_of_space;
//}


namespace simbad{
namespace gui{


class Model_of_space
{
    QString Name_of_model;
    int dimension_of_model;
    int number_of_types_in_model;
//dynamics
    long double Global_clock;
    long double Min_time;

public:
    // All information  about events of model
    // table of component rates
    QVector <Event_of_model> List_of_events_of_model;
    float Proportion_for_radiuses_wrt_Biggest;

    //All information about location +
    //type property(color + number_of_point in type) +  and property of (all type)  points

    Points_of_model ModelPoints;

    //Cell class for searching of paticles
    Cell_Map CellMap;
    const int matrix_scaling_factor = 100;
    const int range_of_call_array=1000000;//10 * 10 * matrix_scaling_factor * matrix_scaling_factor;




    Model_of_space();
    ~Model_of_space();


// functions for changing the Model_of_space


    void set_name_of_model(QString Name_of_the_model);
    void set_number_of_types(int Number);
    void download_model_of_space_from_file(QFile file_sim);
    void initiate_before_simulation();
    void initiale_Dzeta();
    void Event_execution(int Number_of_event, int Number_function_for_component_rate);

    void birth_of_particle(int EventNumber, int Numberfunctionforcomponentrate);
    void birth_of_particle_from_environment(int EventNumber);

    void jump_of_particle(int EventNumber, int Numberfunctionforcomponentrate);
    void mutation_of_particle(int EventNumber, int Numberfunctionforcomponentrate);
    void death_of_particle(int EventNumber, int Numberfunctionforcomponentrate);
    void one_death(int number_of_type_of_death_particle, int number_of_dead_man);
    void one_birth(int number_of_type_of_child,
                   float Xcoordinate_of_new_particle, float Ycoordinate_of_new_particle);
// functions for examining the Model_of_space
    QString get_name_of_model();
    int get_number_of_types();
    void push_back_new_event(const Event_of_model &New_event);
    QVector <Event_of_model> get_vector_of_events();
    QStringList get_model_settings_for_file();
    int get_number_of_dimentsions();


//dynamics
    int Number_of_events_dynamics;
    bool timer();
    //Vector_of_Dzeta boundness
    typedef QVector <long double> Dzeta_of_type_event;
    QVector <Dzeta_of_type_event> Vector_of_Dzeta_type_event;
    float rate_linear_on_configuration(int EventNumber,
                                   int Numberfunctionforcomponentrate,int Number_of_type_of_particle, int Number_of_point_in_type_array);

    //  Vector_max_of_rate_functions
    typedef QVector <double> max_of_rate_functions;
    QVector <max_of_rate_functions> Vector_max_of_rate_functions;

    // times before next events  (type events)
    typedef QVector <long double> Time_before_next_event_of_type;
    QVector <Time_before_next_event_of_type> Vector_time_before_next_event_of_type;


    // comulative rates of type events (type events)
    typedef QVector <long double> Comulative_rate_for_type_of_event;
    QVector <Comulative_rate_for_type_of_event> Vector_comulative_rate_for_type_of_event;

    //seed

    int Number_of_occurings;


    long double get_global_clock();
};//class Model_of_space


}// namespace gui

}// namespace simbad

#endif // MODEL_OF_SPACE_H