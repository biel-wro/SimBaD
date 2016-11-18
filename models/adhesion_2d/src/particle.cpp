#include "particle.hpp"
#include "model_parameters.hpp"

#include <random>

BEGIN_NAMESPACE_ADHESION_2D

particle::particle(position_type p, velocity_type v, acceleration_type acc,
                   time_type et, time_type dt)
    : m_position(std::move(p)),
      m_event_time(std::move(et)),
      m_delta_time(std::move(dt)),
      m_velocity(std::move(v)),
      m_acceleration(std::move(acc))
{
}
particle::~particle() {}
const particle::position_type &particle::position() const { return m_position; }
particle::position_type &particle::position() { return m_position; }
particle::time_type particle::event_time() const { return m_event_time; }
particle::time_type &particle::event_time() { return m_event_time; }
particle::time_type particle::delta_time() const { return m_delta_time; }
particle::time_type &particle::delta_time() { return m_delta_time; }
const particle::velocity_type &particle::velocity() const { return m_velocity; }
particle::velocity_type &particle::velocity() { return m_velocity; }
const particle::acceleration_type &particle::acceleration() const
{
  return m_acceleration;
}

particle::acceleration_type &particle::acceleration() { return m_acceleration; }
particle_view::particle_view(const particle_view::orig *ptr) : m_ptr(ptr) {}
void particle_view::set_orig(const particle_view::orig *ptr) { m_ptr = ptr; }
std::size_t particle_view::dimension() const { return orig::dimension; }
double particle_view::coord(std::size_t d) const
{
  return m_ptr->position()[d];
}

END_NAMESPACE_ADHESION_2D
