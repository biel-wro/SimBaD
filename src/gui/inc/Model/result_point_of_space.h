#ifndef RESULT_POINT_OF_SPACE_H
#define RESULT_POINT_OF_SPACE_H


namespace simbad{
namespace gui{
class result_point_of_space
{
    float Xcoord;
    float Ycoord;
    int type_of_result_point;

public:

    //constructor
        result_point_of_space();
        result_point_of_space (const result_point_of_space& that);
        result_point_of_space & operator = (const result_point_of_space& that)
            {
                Xcoord = that.Xcoord;
                Ycoord = that.Ycoord;
                type_of_result_point= that.type_of_result_point;

                return *this;
            }
     ~result_point_of_space ();
    // Overloading operators

        bool operator == (const result_point_of_space& that)
        {
            if (      this->Xcoord==that.Xcoord &&
                      this->Ycoord==that.Ycoord &&
                      this->type_of_result_point==that.type_of_result_point

               ) return true;

                    return false;
        }




    void set_Xcoord_of_result_point(float coord);
    void set_Ycoord_of_result_point(float coord);
    void set_type_of_result_point(int Type);

    float get_Xcoord_of_result_point();
    float get_Ycoord_of_result_point();
    int get_type_of_result_point();


};



}//namespace simbad{
}//namespace gui{
#endif // RESULT_POINT_OF_SPACE_H
