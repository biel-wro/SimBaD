#include "points_of_model.h"
#include "iostream"
#include "points_of_type.h"
simbad::gui::Points_of_model::Points_of_model() { Number_of_types = 0; }
simbad::gui::Points_of_model::Points_of_model(const Points_of_model &that)
{
  Number_of_types = that.Number_of_types;
  Vector_of_types = that.Vector_of_types;
}
void simbad::gui::Points_of_model::initialisation_of_type(
    int TypeNumber, const QVector<points_of_type> &Array_of_points)
{
  Number_of_types = TypeNumber;
  Vector_of_types = Array_of_points;
}

void simbad::gui::Points_of_model::initialisation_of_model(
    int Number_of_types_in_model)
{
  Number_of_types = Number_of_types_in_model;

  Vector_of_types.resize(Number_of_types);
}
