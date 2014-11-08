#include "inc\Model\result_point_of_space.h"


simbad::gui::result_point_of_space::result_point_of_space()
{
    Xcoord=0;
    Ycoord=0;
    type_of_result_point=-1;
}



simbad::gui::result_point_of_space::result_point_of_space(const result_point_of_space& that) :
    Xcoord(that.Xcoord),
    Ycoord(that.Ycoord),
    type_of_result_point(that.type_of_result_point)
{

}


simbad::gui::result_point_of_space::~result_point_of_space()
{

}


void simbad::gui::result_point_of_space::set_Xcoord_of_result_point(float coord)
{
    Xcoord=coord;
}
void simbad::gui::result_point_of_space::set_Ycoord_of_result_point(float coord)
{
    Ycoord=coord;
}
void simbad::gui::result_point_of_space::set_type_of_result_point(int Type)
{
    type_of_result_point=Type;
}

float simbad::gui::result_point_of_space::get_Xcoord_of_result_point()
{
    return Xcoord;
}
float simbad::gui::result_point_of_space::get_Ycoord_of_result_point()
{
    return Ycoord;
}
int simbad::gui::result_point_of_space::get_type_of_result_point()
{
    return type_of_result_point;
}
