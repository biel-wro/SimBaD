#include "points_of_model.h"
#include "points_of_type.h"
#include"iostream"
simbad::gui::Points_of_model::Points_of_model()
{
    Number_of_types=0;
}



simbad::gui::Points_of_model::Points_of_model(const Points_of_model& that)
{
    Number_of_types=that.Number_of_types;
    Vector_of_types=that.Vector_of_types;

}
void simbad::gui::Points_of_model::initialisation_of_type
                                    (int TypeNumber, const QVector<points_of_type> & Array_of_points)
{
    Number_of_types = TypeNumber;
    Vector_of_types = Array_of_points;
   // cout << "simbad::gui::Points_of_model::initialisation_of_type" << endl;

}

void simbad::gui::Points_of_model::initialisation_of_model(int Number_of_types_in_model)
{
    Number_of_types = Number_of_types_in_model;
 //   cout << "before" << endl;

    Vector_of_types.resize(Number_of_types);
//    for(int i=0;i<Number_of_types_in_model;i++)
//    {
//       points_of_type Points_of_type;

//       Points_of_type.setParticle_Type(i);

//       this->Vector_of_types.push_back(Points_of_type);
//    };
   // cout << "simbad::gui::Points_of_model::initialisation_of_model" << endl;
}