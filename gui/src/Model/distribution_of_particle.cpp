#include "distribution_of_particle.h"



simbad::gui::Distribution_of_particle::Distribution_of_particle()
{

    Distribution_of_particle::Type_of_particle = 0;
    Distribution_of_particle::Type_of_T_student_distribution = 0;

}



simbad::gui::Distribution_of_particle::Distribution_of_particle(int Type_p, int Type_s)
{
    Distribution_of_particle::Type_of_particle = Type_s;
    Distribution_of_particle::Type_of_T_student_distribution = Type_p;

}

simbad::gui::Distribution_of_particle::Distribution_of_particle(const Distribution_of_particle& that)
{

    Type_of_particle = that.Type_of_particle;
    Type_of_T_student_distribution = that.Type_of_T_student_distribution;

}

simbad::gui::Distribution_of_particle& simbad::gui::Distribution_of_particle::operator = (const Distribution_of_particle& that)
{
    Type_of_particle = that.Type_of_particle;
    Type_of_T_student_distribution = that.Type_of_T_student_distribution;

    return *this;


}


int simbad::gui::Distribution_of_particle::get_Type_of_particle()
{
    return Type_of_particle;
}
int simbad::gui::Distribution_of_particle::get_Type_of_T_student_distribution()
{
    return Type_of_T_student_distribution;
}

void simbad::gui::Distribution_of_particle::set_Type_of_particle(int Type_part)
{
    Type_of_particle = Type_part;
}
void simbad::gui::Distribution_of_particle::set_Type_of_Tstudent_distribution(int Type_of_T)
{
    Type_of_T_student_distribution =Type_of_T;
}


