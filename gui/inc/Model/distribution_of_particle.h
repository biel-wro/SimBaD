#ifndef DISTRIBUTION_OF_PARTICLE_H
#define DISTRIBUTION_OF_PARTICLE_H


namespace Ui {
    class Distribution_of_particle;
}




namespace simbad{
namespace gui{



class Distribution_of_particle{
//it will be extended to the vector
    int Type_of_particle;
    int Type_of_T_student_distribution;
    float scale_paramiter;
public:
//constructor
    Distribution_of_particle();
    Distribution_of_particle(int TypeOfParticle,
                             int paramiterofTstudentdistribution,float ScaleParamiter);
    Distribution_of_particle(const Distribution_of_particle& that);

//operators
    Distribution_of_particle& operator = (const Distribution_of_particle& that);
    bool operator == (const Distribution_of_particle& that)
    {
        if (       (Type_of_particle == that.Type_of_particle) and
                   (Type_of_T_student_distribution == that.Type_of_T_student_distribution) and
                   (scale_paramiter == that.scale_paramiter)
            ) return true;

              return false;
    }
    int get_Type_of_particle();
    int get_Type_of_T_student_distribution();
    float get_scale_paramiter();

    void set_Type_of_particle(int Type_part);
    void set_Type_of_Tstudent_distribution(int Type_of_T);
    void set_scale_paramiter(float ScaleParamiter);
};

}//namespace gui
}//namespace simbad


#endif // DISTRIBUTION_OF_PARTICLE_H
