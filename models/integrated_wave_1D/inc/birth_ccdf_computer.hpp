#ifndef BIRTH_CCDF_COMPUTER
#define BIRTH_CCDF_COMPUTER

#include "integrated_wave_1d.hpp"
#include "particle_1d.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D
class birth_ccdf_computer{
public:
    birth_ccdf_computer(double x);
    ~birth_ccdf_computer();

    double operator()(particle_1d const &p) const;
private:
    double x;
};

END_NAMESPACE_INTEGRATED_WAVE_1D

#endif
