#ifndef TABLE_OF_COMPONENT_RATES_H
#define TABLE_OF_COMPONENT_RATES_H
#include"function_for_component_rate.h"
#include <QtCore>
namespace Ui {
    struct Influence_of_environment_and_particles;
    class Table_of_component_rates;
}


namespace simbad{
namespace gui{


struct Influence_of_environment_and_particles
{
    int Number_of_type = 0;
    function_for_component_rate Function_for_rate;
};




class Table_of_component_rates{

    int Type;
    QVector<Influence_of_environment_and_particles> Table;

    public:
//constructor
    Table_of_component_rates(int Number_of_types_in_model_for_component_rates);
    Table_of_component_rates();
};


}//namespace gui
}//namespace simbad


#endif // TABLE_OF_COMPONENT_RATES_H
