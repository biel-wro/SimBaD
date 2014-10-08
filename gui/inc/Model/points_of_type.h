#ifndef POINT_OF_TYPE_H
#define POINT_OF_TYPE_H
#include "QColor"
//#include "point_of_space.h"
#include <vector>
using namespace std;
#include <QtCore>
#include "iostream"
namespace Ui {
    class points_of_type;
}

namespace simbad{
namespace gui{


struct Point_of_space
{
    float Xcoordinate=0.0;
    float Ycoordinate=0.0;
    unsigned int number_of_point_in_cell = 0;
    double BirthTime=0.0;

};


class points_of_type
{


public:
    QColor InitialConfigurationColor;
    QColor DynamicSimulationColor;
    int Particle_Type;

   //it shoud be changed due to the QVarLengthArray Class Reference
//   QVector <Point_of_space> SpacePointArray;
//    Point_of_space SpacePointArray[43000];
    QVector <Point_of_space> SpacePointArray;
//    std::vector <Point_of_space> SpacePointArray;

    //QVector<Point_of_space> Table;
   //--------------------------------------------------------------
   int Number_of_points_in_SpacePointArray;




    points_of_type();
    points_of_type(const points_of_type& that);
    points_of_type& operator = (const points_of_type& that)
            {
                //    SpacePointArray.reserve(1000000);
                    InitialConfigurationColor=that.InitialConfigurationColor;
                    DynamicSimulationColor=that.DynamicSimulationColor;
                    Particle_Type=that.Particle_Type;
                    this->Number_of_points_in_SpacePointArray=that.Number_of_points_in_SpacePointArray;

                    for(int i=0; i<Number_of_points_in_SpacePointArray;i++)
                    {
                        SpacePointArray[i].BirthTime = that.SpacePointArray[i].BirthTime;
                        SpacePointArray[i].number_of_point_in_cell = that.SpacePointArray[i].number_of_point_in_cell;
                        SpacePointArray[i].Xcoordinate = that.SpacePointArray[i].Xcoordinate;
                        SpacePointArray[i].Ycoordinate = that.SpacePointArray[i].Ycoordinate;
                    };
                      cout << "points_of_type& operator = " << endl;
                        return *this;
            }





    // Overloading operators

        bool operator == (const points_of_type& that)
        {

            bool indic = false;
            int Number_of_eq = 0;

            if (       (InitialConfigurationColor  ==  that.InitialConfigurationColor ) and
                       (DynamicSimulationColor ==  that.DynamicSimulationColor)  and
                       (Particle_Type  == that.Particle_Type )

               ){  for(int i=0; i<Number_of_points_in_SpacePointArray;i++)
                   {
                       if( (SpacePointArray[i].BirthTime == that.SpacePointArray[i].BirthTime) and
                           (SpacePointArray[i].number_of_point_in_cell ==
                                that.SpacePointArray[i].number_of_point_in_cell)               and
                           (SpacePointArray[i].Xcoordinate ==
                            that.SpacePointArray[i].Xcoordinate)                               and
                           (SpacePointArray[i].Ycoordinate ==
                            that.SpacePointArray[i].Ycoordinate)
                        )Number_of_eq = Number_of_eq +1;
                }

                    //if (SpacePointArray[i]==that.SpacePointArray[i])


                            if (that.Number_of_points_in_SpacePointArray==Number_of_eq)indic=true;



                    if (indic==true)  return true;
                }
                    return false;
        }
      void setParticle_Type(int ParticleType);
      void setNumber_of_points_in_SpacePointArray(int Number);
//    void setXcoordinate (float Xcoord);
//    void setYcoordinate (float Ycoord);
//    void setCellNumber_of_point (unsigned int Cell_number_of_point);
//    void setBirthTime (double time_of_birth);

//    float getXcoordinate ();
//    float getYcoordinate ();
//    unsigned int getCellNumber_of_point ();
//    double getBirthTime ();



    void Changing_of_color_and_points(int Number_of_new_points, QColor InitConfColor,
    QColor DynSimColor, const QVector<Point_of_space> & Array_of_points);
//destuctor
   ~points_of_type();
};


}//namespace simbad{
}//namespace gui{


#endif // POINT_OF_TYPE_H
