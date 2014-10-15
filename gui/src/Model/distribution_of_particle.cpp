#include "distribution_of_particle.h"



simbad::gui::Distribution_of_particle::Distribution_of_particle()
{

    Distribution_of_particle::Type_of_particle = 0;
    Distribution_of_particle::Type_of_T_student_distribution = 0;
    Distribution_of_particle::scale_paramiter = 0;
}



simbad::gui::Distribution_of_particle::Distribution_of_particle(int TypeOfParticle,
                                                                int paramiterofTstudentdistribution,float ScaleParamiter)
{
    Distribution_of_particle::Type_of_particle = TypeOfParticle;
    Distribution_of_particle::Type_of_T_student_distribution = paramiterofTstudentdistribution;
    Distribution_of_particle::scale_paramiter = ScaleParamiter;
}

simbad::gui::Distribution_of_particle::Distribution_of_particle(const Distribution_of_particle& that)
{

    Type_of_particle = that.Type_of_particle;
    Type_of_T_student_distribution = that.Type_of_T_student_distribution;
    scale_paramiter = that.scale_paramiter;
}

simbad::gui::Distribution_of_particle& simbad::gui::Distribution_of_particle::operator = (const Distribution_of_particle& that)
{
    Type_of_particle = that.Type_of_particle;
    Type_of_T_student_distribution = that.Type_of_T_student_distribution;
    scale_paramiter = that.scale_paramiter;
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

float simbad::gui::Distribution_of_particle::get_scale_paramiter()
{
    return scale_paramiter;
}


void simbad::gui::Distribution_of_particle::set_Type_of_particle(int Type_part)
{
    Type_of_particle = Type_part;
}
void simbad::gui::Distribution_of_particle::set_Type_of_Tstudent_distribution(int Type_of_T)
{
    Type_of_T_student_distribution =Type_of_T;
}


void simbad::gui::Distribution_of_particle::set_scale_paramiter(float ScaleParamiter)
{
    scale_paramiter = ScaleParamiter;
}
