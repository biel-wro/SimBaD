#include "function_for_component_rate.h"

simbad::gui::function_for_component_rate::function_for_component_rate()
{
    Function_for_rate=null;
    Multiplication_by_constant=0;
    Restriction_of_the_range=zero;
    Range_of_the_function=0;
    Approach=no;

}

simbad::gui::function_for_component_rate::function_for_component_rate(const function_for_component_rate& that) :
    Function_for_rate(that.Function_for_rate),
    Multiplication_by_constant(that.Multiplication_by_constant),
    Restriction_of_the_range(that.Restriction_of_the_range),
    Range_of_the_function(that.Range_of_the_function),
    Approach(that.Approach)
{

}

void simbad::gui::function_for_component_rate::set_Functions_from_string(QString A)
{

        if (A == "-- 0")
        {
            Function_for_rate = null;
        };

        if (A == "-- 1 - d if d \u2208 [0, 1),\n    0 if d \u2208 [1, \u221E)")
        {
            Function_for_rate = triangle;
        };

        if (A == "-- d if d \u2208 [0; +\u221E)")
        {
            Function_for_rate = modul;
        };

        if (A == "-- 1")
        {
            Function_for_rate = one;
        };

        if (A == "-- exp(-d) if d \u2208 [0; +\u221E)")
        {
            Function_for_rate=exp_minus;
        };

        if (A == "-- 0 if d \u2208 [0,1)\n    d - 1 if d \u2208 [1; +\u221E)")
        {
            Function_for_rate=zero_next_to_null;
        };

        if (A == "-- min(d)")
        {
            Function_for_rate=min;
        };

        if ( (A != "-- min(d)") and
             (A != "-- 0 if d \u2208 [0,1)\n    d - 1 if d \u2208 [1; +\u221E)") and
             (A != "-- exp(-d) if d \u2208 [0; +\u221E)") and
             (A != "-- 1") and
             (A != "-- d if d \u2208 [0; +\u221E)") and
             (A != "-- 1 - d if d \u2208 [0, 1),\n    0 if d \u2208 [1, \u221E)") and
             (A != "-- 0"))
        {
            Function_for_rate=null;
        };

}

void simbad::gui::function_for_component_rate::set_Restrictions_from_string(QString A)
{
        if (A == "zero")
        {
            Restriction_of_the_range = zero;
        };

        if (A == "circle")
        {
            Restriction_of_the_range = circle;
        };

        if (A == "square")
        {
            Restriction_of_the_range = square;
        };

        if ((A != "zero") and
            (A != "circle") and
            (A != "square"))
        {
            Restriction_of_the_range = zero;
        };
}

void simbad::gui::function_for_component_rate::set_Approach_of_influence_from_string(QString A)
{


        if (A=="no")
        {
            Approach = no;
        };

        if (A=="\u2211")
        {
            Approach= sum;
        };

        if (A=="exp(-\u2211)")
        {
            Approach = exp_minus_sum;
        };

        if (A=="exp(+\u2211)")
        {
            Approach = exp_plus_sum;
        };

        if (A=="\u220F")
        {
            Approach = product;
        };

        if ( (A!="no") and
             (A!="\u2211") and
             (A!="exp(-\u2211)")and
             (A!="exp(+\u2211)") and
             (A!="\u220F")
            )
        {
            Approach = no;
        };
}

void simbad::gui::function_for_component_rate::set_Multiplication_by_constant(float value)
{
    Multiplication_by_constant = value;
}

void simbad::gui::function_for_component_rate::set_Range_of_the_function(float value)
{
    Range_of_the_function = value;
}


QString simbad::gui::function_for_component_rate::get_string_of_Functions()
{


    if (Function_for_rate == null )
    {
        return "-- 0";
    };

    if (Function_for_rate == triangle)
    {
        return  "-- 1 - d if d \u2208 [0, 1),\n    0 if d \u2208 [1, \u221E)";
    };

    if (Function_for_rate == modul)
    {
       return "-- d if d \u2208 [0; +\u221E)";
    };

    if (Function_for_rate == one)
    {
        return "-- 1";
    };

    if (Function_for_rate == exp_minus)
    {
        return "-- exp(-d) if d \u2208 [0; +\u221E)";
    };

    if (Function_for_rate == zero_next_to_null)
    {
        return "-- 0 if d \u2208 [0,1)\n    d - 1 if d \u2208 [1; +\u221E)";
    };

    if (Function_for_rate == min)
    {
        return "-- min(d)";
    };

    return "-- 0";


}

QString simbad::gui::function_for_component_rate::get_string_of_Restrictions()
{


    if (Restriction_of_the_range == zero)
    {
        return "zero";
    };
    if (Restriction_of_the_range == circle)
    {
        return "circle";
    };
    if (Restriction_of_the_range == square)
    {
        return "square";
    };

    return "zero";

}

QString simbad::gui::function_for_component_rate::get_string_of_Approach_of_influence()
{

    if (Approach == no)
    {
        return "no";
    };

    if (Approach == sum)
    {
        return "\u2211";
    };

    if (Approach == exp_minus_sum)
    {
        return "exp(-\u2211)";
    };

    if (Approach == exp_plus_sum)
    {
        return "exp(+\u2211)";
    };

    if (Approach == product)
    {
        return "\u220F";
    };

    return "no";

}

float  simbad::gui::function_for_component_rate::get_Multiplication_of_constant()
{
    return Multiplication_by_constant;
}

float  simbad::gui::function_for_component_rate::get_Range_of_function()
{
    return Range_of_the_function;
}

simbad::gui::Functions simbad::gui::function_for_component_rate::get_Functions()
{
    return Function_for_rate;
}

simbad::gui::Restrictions simbad::gui::function_for_component_rate::get_Restrictions()
{
    return Restriction_of_the_range;
}

simbad::gui::Approach_of_influence simbad::gui::function_for_component_rate::get_Approach_of_influence()
{
    return Approach;
}

