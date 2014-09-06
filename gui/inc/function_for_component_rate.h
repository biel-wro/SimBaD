#ifndef FUNCTION_FOR_COMPONENT_RATE_H
#define FUNCTION_FOR_COMPONENT_RATE_H



//enum Functions_of_rates  1="-- 0",
//                         2="-- 1 - d if d \u2208 [0, 1),\n    0 if d \u2208 [1, \u221E)",
//                         3="-- d if d \u2208 [0; +\u221E)",
//                         4="-- 1",
//                         5="-- exp(-d) if d \u2208 [0; +\u221E)",
//                         6="-- 0 if d \u2208 [0,1)\n    d - 1 if d \u2208 [1; +\u221E)",
//                         7="-- 0 if d \u2208 [0,1)\n    d - 1 if d \u2208 [1; +\u221E)",
//                         8="-- min(d)";

namespace Ui {
    class function_for_component_rate;
}

namespace simbad{
namespace gui{

enum Functions{
    null,
    triangle,
    modul,
    one,
    exp_minus,
    zero_next_to_null
};

enum Restrictions{
    zero,
    circle,
    square
};


enum Approach_of_influence{
    no,
    sum,
    exp_minus_sum,
    exp_plus_sum,
    product
};


class function_for_component_rate
{
    Functions Function_for_rate;
    float Multiplication_by_constant;
    Restrictions Restriction_of_the_range;
    float Range_of_the_function;
    Approach_of_influence Approach;
public:
//constructor
    function_for_component_rate ();

};






}//namespace gui
}//namespace simbad

#endif // FUNCTION_FOR_COMPONENT_RATE_H
