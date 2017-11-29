#ifndef POINT_OF_SPACE_H
#define POINT_OF_SPACE_H
#include <QtCore>

//namespace Ui {
//    class Point_of_space;
//}


//namespace simbad{
//namespace gui{



class Point_of_space
{
    float Xcoordinate;
    float Ycoordinate;
    unsigned int number_of_point_in_cell;
    long double BirthTime;

public:

//+++
//    int Xcoordinate;
//    int Ycoordinate;
//++


    //constructor


    Point_of_space ();
    Point_of_space (const Point_of_space& that);
    Point_of_space& operator = (const Point_of_space& that)
            {
                Xcoordinate = that.Xcoordinate;
                Ycoordinate = that.Ycoordinate;
                number_of_point_in_cell = that.number_of_point_in_cell;
                BirthTime = that.BirthTime;

                return *this;
            }





    // Overloading operators

        bool operator == (const Point_of_space& that)
        {
            if (       (this->Xcoordinate  ==  that.Xcoordinate ) and
                       (this->Ycoordinate  ==  that.Ycoordinate )  and
                       (this->number_of_point_in_cell  == that.number_of_point_in_cell )  and
                       (this->BirthTime == that.BirthTime)
               ) return true;

                    return false;
        }
//+++
    void setXcoordinate (float Xcoord);
    void setYcoordinate (float Ycoord);
//+++
    void setCellNumber_of_point (unsigned int Cell_number_of_point);
    void setBirthTime (double time_of_birth);

    float getXcoordinate ();
    float getYcoordinate ();
    unsigned int getCellNumber_of_point ();
    double getBirthTime ();
//destructor
    ~Point_of_space ();

};




//}//namespace simbad{
//}//namespace gui{




#endif // POINT_OF_SPACE_H
