#include "table_of_component_rates.h"

simbad::gui::Table_of_component_rates::Table_of_component_rates() {}
simbad::gui::Table_of_component_rates::Table_of_component_rates(
    int Number_of_types_in_model_for_component_rates)
{
  Number_of_types = Number_of_types_in_model_for_component_rates;
}

simbad::gui::Table_of_component_rates::Table_of_component_rates(
    const Table_of_component_rates &that)
{
  this->Number_of_types = that.Number_of_types;
  this->Table = that.Table;
}
