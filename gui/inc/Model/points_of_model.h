#ifndef POINTS_OF_MODEL_H
#define POINTS_OF_MODEL_H
#include "points_of_type.h"


namespace simbad{
namespace gui{



class Points_of_model
{

public:
    int Number_of_types;
    QVector <points_of_type> Vector_of_types;



    Points_of_model();
    Points_of_model(const Points_of_model& that);
    Points_of_model& operator = (const Points_of_model& that)
            {
                    Number_of_types=that.Number_of_types;
                    Vector_of_types=that.Vector_of_types;

                    return *this;
            }

    void initialisation_of_model(int Number_of_types_in_model);

    void initialisation_of_type(int TypeNumber, const QVector<points_of_type> & Array_of_types);
};


}//namespace simbad{
}//namespace gui{



#endif // POINTS_OF_MODEL_H
