#ifndef PARTICLE_1D_HPP
#define PARTICLE_1D_HPP

#include <boost/intrusive/set_hook.hpp>
#include <random>

#include "event_kind.hpp"
#include "simple_particle.hpp"

#include "density_accumulator.hpp"
#include "integrated_wave_1d.hpp"
#include "integrated_wave_1d_fwd.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

class particle_1d : public ::simbad::core::simple_particle<1, float>,
                    public boost::intrusive::set_base_hook<
                        boost::intrusive::optimize_size<true>>
{
    using particle_super = simbad::core::simple_particle<1, float>;
    using hook_super =
        boost::intrusive::set_base_hook<boost::intrusive::optimize_size<true>>;

  public:
    explicit particle_1d(float pos = 0.f);
    ~particle_1d();

    void set_position(float pos);
    float get_position() const;

    void update(event_1d const &e);
    void update(particle_1d const &neighbor);

    density_accumulator const &get_accumulator() const;

  protected:
    density_accumulator acc;
};

END_NAMESPACE_INTEGRATED_WAVE_1D

#endif // PARTICLE_1D_HPP
