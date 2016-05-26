#ifndef SATURATION_DISTANCE_OBSERVER_HPP
#define SATURATION_DISTANCE_OBSERVER_HPP

#include "front_wave_1d_fwd.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D
class saturation_distance_computer
{
  public:
    double observe(front_wave_1d_impl const &model) const;
};
END_NAMESPACE_FRONT_WAVE_1D
#endif // SATURATION_DISTANCE_OBSERVER_HPP
