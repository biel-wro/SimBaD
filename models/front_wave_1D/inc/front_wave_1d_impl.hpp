#ifndef FRONT_WAVE_1D_IMPL_H
#define FRONT_WAVE_1D_IMPL_H
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
class front_wave_1d_impl
{
  public:
    using Queue = simbad::core::simple_event_queue;
    using EventHandle = simbad::core::simple_event_handle;

    using Event = simbad::core::simple_event;
    using EVENT_KIND = simbad::core::EVENT_KIND;

    using Random = std::mt19937_64;
    using Space = boost::intrusive::set<particle_1D>;

    front_wave_1d_impl();
    ~front_wave_1d_impl();

    void reinitialize();
    std::pair<float, EVENT_KIND> next_event();

  protected:
    Random rnd;
    Queue queue;
    Space space;

    double t;
};
}
}
#endif // FRONT_WAVE_1D_IMPL_H
