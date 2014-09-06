#ifndef DISTRIBUTION_OF_PARTICLE_H
#define DISTRIBUTION_OF_PARTICLE_H


namespace Ui {
    class Distribution_of_particle;
}




namespace simbad{
namespace gui{



class Distribution_of_particle{
    int Type_of_particle;
    int Type_of_T_student_distribution;
public:
//constructor
    Distribution_of_particle();
    Distribution_of_particle(int Type_p, int  Type_s);

};

}//namespace gui
}//namespace simbad


#endif // DISTRIBUTION_OF_PARTICLE_H
