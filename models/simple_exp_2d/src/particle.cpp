#include "particle.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D

particle::particle(particle::queue_handle_type h) : self_base_type(std::move(h)) {}

particle::particle(particle::space_coords sc, particle::queue_handle_type h)
    : self_base_type(std::move(h)), m_coords(std::move(sc))
{
}

void particle::set_handle(particle::queue_handle_type handle)
{
  self_base_type::set_handle(std::move(handle));
}

particle::queue_handle_type particle::get_handle() const
{
  return self_base_type::get_handle();
}

particle::space_coords &particle::coords() { return m_coords; }

const particle::space_coords &particle::coords() const { return m_coords; }

END_NAMESPACE_SIMPLE_EXP_2D
