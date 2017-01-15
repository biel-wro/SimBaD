#include "generic_particle.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_list.hpp"

#include <tuple>

BEGIN_NAMESPACE_CORE

generic_particle::generic_particle() {}
void generic_particle::visit_view(
    configuration_view::particle_visitor v) const
{
  struct : attribute_list
  {
    generic_particle const *self;
    attribute get_attribute(std::size_t idx) const override
    {
      return self->attributes()[idx];
    }
  } particle_view;
  particle_view.self = this;
  v(particle_view);
}
void generic_particle::clear_attributes() { m_attributes.clear(); }
void generic_particle::set_attribute(std::size_t idx, attribute attr)
{
  if(m_attributes.size() <= idx)
    m_attributes.resize(idx + 1);
  m_attributes[idx] = std::move(attr);
}
const attribute &generic_particle::find_attribute(std::size_t idx) const
{
  return m_attributes[idx];
}

std::vector<attribute> &generic_particle::attributes() { return m_attributes; }
std::vector<attribute> const &generic_particle::attributes() const
{
  return m_attributes;
}

bool generic_particle::operator==(const generic_particle &gp) const
{
  return gp.attributes()[0] == attributes()[0];
}

bool generic_particle::operator!=(const generic_particle &gp) const
{
  return !this->operator==(gp);
}

END_NAMESPACE_CORE
std::size_t std::hash<simbad::core::generic_particle>::
operator()(const simbad::core::generic_particle &p) const
{
  std::hash<simbad::core::attribute> hasher;
  return hasher(p.attributes()[0]);
}
