#include "point_of_space.h"

simbad::gui::Point_of_space::Point_of_space()
{
  Xcoordinate = 0.0;
  Ycoordinate = 0.0;
  number_of_point_in_cell = 0;
  BirthTime = 0.0;
}

simbad::gui::Point_of_space::Point_of_space(const Point_of_space &that)
    : Xcoordinate(that.Xcoordinate),
      Ycoordinate(that.Ycoordinate),
      number_of_point_in_cell(that.number_of_point_in_cell),
      BirthTime(that.BirthTime)
{
}

//+++
void simbad::gui::Point_of_space::setXcoordinate(float Xcoord)
//+++
{
  Xcoordinate = Xcoord;
}

//+++
void simbad::gui::Point_of_space::setYcoordinate(float Ycoord)
//+++
{
  Ycoordinate = Ycoord;
}

void simbad::gui::Point_of_space::setCellNumber_of_point(
    unsigned int Cell_number_of_point)
{
  number_of_point_in_cell = Cell_number_of_point;
}

void simbad::gui::Point_of_space::setBirthTime(double time_of_birth)
{
  BirthTime = time_of_birth;
}

float simbad::gui::Point_of_space::getXcoordinate() { return Xcoordinate; }
float simbad::gui::Point_of_space::getYcoordinate() { return Ycoordinate; }
unsigned int simbad::gui::Point_of_space::getCellNumber_of_point()
{
  return number_of_point_in_cell;
}

double simbad::gui::Point_of_space::getBirthTime() { return BirthTime; }
