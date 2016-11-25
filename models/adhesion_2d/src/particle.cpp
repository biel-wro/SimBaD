#include "particle.hpp"
#include "model_parameters.hpp"

#include <random>

BEGIN_NAMESPACE_ADHESION_2D

cell::cell(position_type p, velocity_type v,
           time_type et, time_type dt)
    : m_position(std::move(p)),
      m_event_time(std::move(et)),
      m_delta_time(std::move(dt)),
      m_velocity(std::move(v))
{
}
cell::~cell() {}
const cell::position_type &cell::position() const { return m_position; }
cell::position_type &cell::position() { return m_position; }
cell::time_type cell::event_time() const { return m_event_time; }
cell::time_type &cell::event_time() { return m_event_time; }
cell::time_type cell::delta_time() const { return m_delta_time; }
cell::time_type &cell::delta_time() { return m_delta_time; }
const cell::velocity_type &cell::velocity() const { return m_velocity; }
cell::velocity_type &cell::velocity() { return m_velocity; }
// const cell::acceleration_type &cell::acceleration() const
//{
//  return m_acceleration;
//}
// cell::acceleration_type &cell::acceleration() { return m_acceleration; }
particle_view::particle_view(const particle_view::orig *ptr) : m_ptr(ptr) {}
void particle_view::set_orig(const particle_view::orig *ptr) { m_ptr = ptr; }
std::size_t particle_view::dimension() const { return orig::dimension; }
double particle_view::coord(std::size_t d) const
{
  return m_ptr->position()[d];
}

simbad::core::attribute
particle_view::extra_attribute(const std::string &attribute_name) const
{
  if("velocity_x" == attribute_name)
    return m_ptr->velocity()[0];
  else if("velocity_y" == attribute_name)
    return m_ptr->velocity()[1];

  std::string msg = "attribute '" + attribute_name + "' not found";
  throw std::runtime_error(std::move(msg));
}

END_NAMESPACE_ADHESION_2D
