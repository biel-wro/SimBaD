#ifndef INTENSITY_COMPUTER_HPP
#define INTENSITY_COMPUTER_HPP
#include "integrated_wave_1d.hpp"
#include "integrated_wave_1d_fwd.hpp"

#include "particle_1d.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

class intensity_computer
{
  public:
    intensity_computer(double density_limit) : denstiy_limit(density_limit) {}
    double operator()(particle_1d const &p ) const
    {
        double density = p.get_accumulator().get_value();
        return density > denstiy_limit ? 1 : 2;
    }

  protected:
    const double denstiy_limit;
};

END_NAMESPACE_INTEGRATED_WAVE_1D

#endif // INTENSITY_COMPUTER_HPP
