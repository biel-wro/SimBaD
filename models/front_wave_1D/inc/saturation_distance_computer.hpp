#ifndef SATURATION_DISTANCE_OBSERVER_HPP
#define SATURATION_DISTANCE_OBSERVER_HPP

#include "front_wave_1d_fwd.hpp"

namespace simbad
{
namespace models
{
class saturation_distance_computer
{
  public:
    double observe(front_wave_1d_impl const &model) const;
};
}
}

#endif // SATURATION_DISTANCE_OBSERVER_HPP
