#include "points_of_type.h"
#include <iostream>

simbad::gui::points_of_type::points_of_type()
{

    InitialConfigurationColor=Qt::black;
    DynamicSimulationColor=Qt::gray;
    Particle_Type=0;
    Number_of_points_in_SpacePointArray=0;
    //SpacePointArray.reserve(40000);
    //qwe.reserve(400000);
    SpacePointArray.resize(100000);
}

simbad::gui::points_of_type::points_of_type (const points_of_type& that)
{
    SpacePointArray.resize(100000);
    InitialConfigurationColor=that.InitialConfigurationColor;
    DynamicSimulationColor=that.DynamicSimulationColor;
    Particle_Type=that.Particle_Type;

    Number_of_points_in_SpacePointArray=that.Number_of_points_in_SpacePointArray;
    for(int i=0; i<Number_of_points_in_SpacePointArray;i++){
        SpacePointArray[i].BirthTime=that.SpacePointArray[i].BirthTime;
        SpacePointArray[i].number_of_point_in_cell=that.SpacePointArray[i].number_of_point_in_cell;
        SpacePointArray[i].Xcoordinate=that.SpacePointArray[i].Xcoordinate;
        SpacePointArray[i].Ycoordinate=that.SpacePointArray[i].Ycoordinate;
    }

}


void simbad::gui::points_of_type::Changing_of_color_and_points(int Number_of_points, QColor InitConfColor,
QColor DynSimColor, const QVector<Point_of_space> & Array_of_points)

{
    InitialConfigurationColor = InitConfColor;
    DynamicSimulationColor = DynSimColor;

    for(int i=0;i<Number_of_points;i++ )
   {
        SpacePointArray[Number_of_points_in_SpacePointArray + i].BirthTime=Array_of_points.at(i).BirthTime;
        SpacePointArray[Number_of_points_in_SpacePointArray + i].number_of_point_in_cell=Array_of_points.at(i).number_of_point_in_cell;
        SpacePointArray[Number_of_points_in_SpacePointArray + i].Xcoordinate=Array_of_points.at(i).Xcoordinate;
        SpacePointArray[Number_of_points_in_SpacePointArray + i].Ycoordinate=Array_of_points.at(i).Ycoordinate;

   }

    Number_of_points_in_SpacePointArray = Number_of_points_in_SpacePointArray + Number_of_points;

}

void simbad::gui::points_of_type::setParticle_Type(int ParticleType)
{
    Particle_Type = ParticleType;
}

void simbad::gui::points_of_type::setNumber_of_points_in_SpacePointArray(int Number)
{
    Number_of_points_in_SpacePointArray = Number;
}


simbad::gui::points_of_type::~points_of_type()
{
}


int simbad::gui::points_of_type::get_number_of_points_in_type()
{
    return Number_of_points_in_SpacePointArray;
}
