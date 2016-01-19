#ifndef FRONT_WAVE_1D_IMPL_H
#define FRONT_WAVE_1D_IMPL_H
#include <random>
#include <utility>

#include "core_fwd.hpp"

#include "front_wave_1d_fwd.hpp"
#include "offspring_placer.hpp"
#include "particle_1d.hpp"
#include "simple_event.hpp"
#include "space_1d.hpp"
#include "spatial_neighbourhood.hpp"

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
    using Event = event_1d;

    using Random = std::mt19937_64;
    using Space = space_1d;

    front_wave_1d_impl();
    ~front_wave_1d_impl();

    void clear();
    void reinitialize();

    Event next_event();

    Queue::ordered_iterator begin_queue() const;
    Queue::ordered_iterator end_queue() const;

    std::size_t size() const;
    double simulation_time() const;
    Space::const_iterator begin() const;
    Space::const_iterator end() const;

  protected:
    void resample_event(particle_1D &p);
    void init_configuration();
    void init_event_queue();

    void update_neighbourhood(Event const &e);
    void full_update(particle_1D &p);

    void update_simulation_time(double new_time);
    Event execute_event();

    Event execute_birth();
    Event execute_death();

    spatial_neighbourhood get_neighbourhood(float center);

  private:
    double t;

    offspring_placer placer;
    Random rnd;
    Queue queue;
    Space space;
};
}
}
#endif // FRONT_WAVE_1D_IMPL_H
