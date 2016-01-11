#ifndef FRONT_WAVE_1D_ALGORITHMS_HPP
#define FRONT_WAVE_1D_ALGORITHMS_HPP

#include <random>
#include <utility>

#include <boost/intrusive/set.hpp>

#include "core_fwd.hpp"
#include "lazy_set.hpp"

#include "front_wave_1d_fwd.hpp"
#include "particle_1d.hpp"

namespace simbad
{
namespace models
{

class front_wave_1d_algorithms
{
  public:
    front_wave_1d_algorithms();
    static int run();

    using Queue = simbad::core::simple_event_queue;
    using EventHandle = simbad::core::simple_event_handle;

    using Event = simbad::core::simple_event;
    using EVENT_KIND = simbad::core::EVENT_KIND;

    using Random = std::mt19937_64;
    using Storage = simbad::core::LazySet<particle_1D>;
    using Space = boost::intrusive::set<particle_1D>;

    using spatial_iterator = Space::iterator;
    using const_spatial_iterator = Space::const_iterator;

    static std::pair<Storage, Space> initial_configuration();

    static Event compute_event(Random &rnd, particle_1D const &p,
                                    Space const &space);

    static Queue init_event_queue(Random &rnd, Space &space);
    static void update_neighbourhood(Random &rnd, Queue &eq,
                                            double center, double range,
                                            Space const &space,
                                            double time_offset);

    static void execute_death(Storage &storage, Space &space, Queue &eq);

    static particle_1D &execute_birth(std::mt19937_64 &rnd, Storage &storage,
                                      Space &space, Queue &eq);

    static void execute_event(double &t, Random &rnd, Storage &storage,
                              Space &space, Queue &eq);
};
}
}
#endif // FRONT_WAVE_1D_ALGORITHMS_HPP
