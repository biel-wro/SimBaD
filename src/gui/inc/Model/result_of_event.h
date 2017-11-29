#ifndef RESULT_OF_EVENT_H
#define RESULT_OF_EVENT_H
#include "result_point_of_space.h"
#include<QVector>


namespace simbad{
namespace gui{

class result_of_event
{
    long double global_time_of_event;
    QVector<result_point_of_space> Vector_of_new_points;
    QVector<result_point_of_space> Vector_of_dead_points;


public:
    //constructor


    result_of_event();
    result_of_event(int time, QVector<result_point_of_space> New_points, QVector<result_point_of_space> Dead_points);
    result_of_event(const result_of_event &that);

    result_of_event & operator = (const result_of_event& that)
            {
                this->global_time_of_event =
                 that.global_time_of_event;
                this->Vector_of_new_points = that.Vector_of_new_points;
                this->Vector_of_dead_points = that.Vector_of_dead_points;
//                for(int i=0; i < that.Vector_of_new_points.size(); i++)
//                {
//                    this->Vector_of_new_points.push_back(that.Vector_of_new_points[i]);
//                };
//                for(int i=0; i < that.Vector_of_dead_points.size(); i++)
//                {
//                    this->Vector_of_dead_points.push_back(that.Vector_of_dead_points[i]);
//                };

                return *this;
            }
     ~result_of_event();
    // Overloading operators

//        bool operator == (const result_of_event& that)
//        {
//            if (
//                    this->global_time_of_event==that.global_time_of_event &&
//                    this->Vector_of_dead_points == that.Vector_of_dead_points &&
//                    this->Vector_of_new_points == that.Vector_of_new_points
//               ) return true;

//                    return false;
//        }




        void set_global_time_of_result_event(long double time);
        long double get_global_time_of_result_event();


        QVector<simbad::gui::result_point_of_space> get_vector_of_new_points();
        QVector<simbad::gui::result_point_of_space> get_vector_of_dead_points();






        void set_vector_of_new_points(QVector<result_point_of_space> New_points);
        void set_vector_of_dead_points(QVector<result_point_of_space> Dead_points);
};


}//simbad
}//gui

#endif // RESULT_OF_EVENT_H
