#ifndef FRONT_WAVE_1D_IMPL_H
#define FRONT_WAVE_1D_IMPL_H
#include <random>
#include <utility>

#include <boost/intrusive/set.hpp>

#include "core_fwd.hpp"
#include "lazy_set.hpp"
#include "offspring_placer.hpp"
#include "simple_event.hpp"

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

    using EventSchedule = simbad::core::simple_event_schedule;
    using EVENT_KIND = simbad::core::EVENT_KIND;
    using Event = core::simple_event<float, float, 1>;

    using Random = std::mt19937_64;
    using Space = boost::intrusive::set<particle_1D>;

    front_wave_1d_impl();
    ~front_wave_1d_impl();

    void reinitialize();

    Event next_event();

  protected:
    double default_interation_range();

    Space initial_configuration();
    Queue init_event_queue();

    EventSchedule compute_event(particle_1D const &p);
    void update_neighbourhood(double center);

    Event execute_event();

    particle_1D &execute_birth();
    void execute_death();

  private:
    double t;
    double range;

    offspring_placer op;
    Random rnd;
    Queue queue;
    Space space;

};
}
}
#endif // FRONT_WAVE_1D_IMPL_H
