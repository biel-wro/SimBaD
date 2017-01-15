#include "particle.hpp"
#include "interface/attribute.hpp"
#include "model_parameters.hpp"
#include "utils/attribute_converter.hpp"
#include <random>

BEGIN_NAMESPACE_ADHESION_2D

cell::cell(position_type p, velocity_type v, time_type et, time_type dt)
    : m_position(std::move(p)),

      m_velocity(std::move(v)),
      m_force(0),
      m_next_time(std::move(et)),
      m_delta_time(std::move(dt)),
      m_pressure(0)
{
}
cell::~cell() {}
const cell::position_type &cell::position() const { return m_position; }
cell::position_type &cell::position() { return m_position; }
cell::time_type cell::next_jump_time() const { return m_next_time; }
cell::time_type &cell::next_jump_time() { return m_next_time; }
cell::time_type cell::prev_jump_time() const { return m_prev_time; }
cell::time_type &cell::prev_jump_time() { return m_prev_time; }
cell::time_type cell::delta_time() const { return m_next_time - m_prev_time; }
const cell::velocity_type &cell::velocity() const { return m_velocity; }
cell::velocity_type &cell::velocity() { return m_velocity; }
const cell::force_type &cell::force() const { return m_force; }
cell::force_type &cell::force() { return m_force; }
cell::pressure_type cell::pressure() const { return m_pressure; }
cell::pressure_type &cell::pressure() { return m_pressure; }
/*
 * particle_view
 */
particle_view::particle_view(const orig &c) : m_cell(c) {}
double particle_view::coord(std::size_t d) const
{
  return m_cell.position()[d];
}

new_particle_view::new_particle_view(const new_particle_view::base &base_ref)
    : m_base_p(base_ref)
{
}

core::attribute new_particle_view::get_attribute(std::size_t idx) const
{
  assert(0 == idx);
  return simbad::core::attribute_converter::convert_to<
      simbad::core::coordinates<double, 2>>(m_base_p.position()).get();
}

/*
simbad::core::attribute
particle_view::extra_attribute(std::size_t attribute_name) const
{
  std::string msg = "attribute '" + attribute_name + "' not found";
  throw std::runtime_error(std::move(msg));
}*/

END_NAMESPACE_ADHESION_2D
