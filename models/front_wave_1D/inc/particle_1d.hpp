#ifndef PARTICLE_1D_HPP
#define PARTICLE_1D_HPP
#include <boost/intrusive/set_hook.hpp>

#include "simple_event_queue.hpp"
#include "simple_particle.hpp"

namespace simbad
{
namespace models
{

class particle_1D : public ::simbad::core::simple_particle<1, float>,
                    public boost::intrusive::set_base_hook<
                        boost::intrusive::optimize_size<true>>
{
    using particle_super = simbad::core::simple_particle<1, float>;
    using hook_super =
        boost::intrusive::set_base_hook<boost::intrusive::optimize_size<true>>;

    using event_handle = simbad::core::simple_event_queue::handle_type;

  public:
    explicit particle_1D(float pos = 0.f, event_handle h = event_handle());
};
}
}
#endif // PARTICLE_1D_HPP
