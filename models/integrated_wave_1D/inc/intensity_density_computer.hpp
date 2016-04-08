#ifndef INTENSITY_DENSITY_COMPUTER_HPP
#define INTENSITY_DENSITY_COMPUTER_HPP

#include "integrated_wave_1d.hpp"

#include "particle_1d.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

class intensity_density_computer
{
public:
    intensity_density_computer(double x);
    ~intensity_density_computer();
    double operator()(particle_1d const &p) const;
private:
    double x;
};

END_NAMESPACE_INTEGRATED_WAVE_1D

#endif // INTENSITY_DENSITY_COMPUTER_HPP
