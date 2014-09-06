#include "distribution_of_particle.h"

simbad::gui::Distribution_of_particle::Distribution_of_particle(int Type_p, int Type_s)
{
    Distribution_of_particle::Type_of_particle = Type_s;
    Distribution_of_particle::Type_of_T_student_distribution = Type_p;

}

simbad::gui::Distribution_of_particle::Distribution_of_particle()
{

    Distribution_of_particle::Type_of_particle = 0;
    Distribution_of_particle::Type_of_T_student_distribution = 0;

}
