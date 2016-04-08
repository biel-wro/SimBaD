#ifndef PARTICLE_1D_HPP
#define PARTICLE_1D_HPP

#include <random>
#include <boost/intrusive/set_hook.hpp>

#include "birth_rate_accumulator.hpp"
#include "death_rate_accumulator.hpp"
#include "event_rate_accumulator.hpp"
#include "simple_event_queue.hpp"
#include "simple_tracked_particle.hpp"

#include "front_wave_1d_fwd.hpp"

namespace simbad
{
namespace models
{

class particle_1D : public ::simbad::core::simple_tracked_particle<1, float>,
                    public boost::intrusive::set_base_hook<
                        boost::intrusive::optimize_size<true>>
{
    using particle_super = simbad::core::simple_tracked_particle<1, float>;
    using hook_super =
        boost::intrusive::set_base_hook<boost::intrusive::optimize_size<true>>;

    using event_handle = simbad::core::simple_event_queue::handle_type;

  public:
    explicit particle_1D(float pos = 0.f, event_handle h = event_handle());
    event_rate_accumulator const &get_event_acc() const;
    void update_accumulators(event_1d const &e);
    void update_accumulators(particle_1D const &p);

    static double interaction_range();
    std::pair<float,simbad::core::EVENT_KIND> sample_event(std::mt19937_64 &r);

protected:
    event_rate_accumulator acc;

};
}
}
#endif // PARTICLE_1D_HPP
