#include "table_of_component_rates.h"



simbad::gui::Table_of_component_rates::Table_of_component_rates(int Number_of_types_in_model_for_component_rates)
{
    for (int i=0 ; i < Number_of_types_in_model_for_component_rates;i++){
        Influence_of_environment_and_particles A;
        A.Number_of_type=i;
        Table_of_component_rates::Table.push_back(A);
    };
}

simbad::gui::Table_of_component_rates::Table_of_component_rates()
{
}
