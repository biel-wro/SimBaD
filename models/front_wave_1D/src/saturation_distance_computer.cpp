#include "saturation_distance_computer.hpp"

#include <limits>

#include "front_wave_1d_impl.hpp"
#include "particle_1d.hpp"


namespace simbad
{
namespace models
{
double
saturation_distance_computer::observe(const front_wave_1d_impl &model) const
{
    double top_dist = -std::numeric_limits<double>::infinity();
    for (particle_1D const &particle : model)
    {
        double dist = (particle.get_coordinate<0>());

        bool is_saturated = particle.get_event_acc().is_saturated();

        if (dist > top_dist && is_saturated)
            top_dist = dist;
    }

    return top_dist;
}
}
}
