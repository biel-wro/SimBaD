#include "generic_particle.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"

#include <tuple>

BEGIN_NAMESPACE_CORE

generic_particle::generic_particle() {}
void generic_particle::visit_view(configuration_view::particle_visitor v,
                                  std::size_t id_attr_idx,
                                  std::size_t coord_attr_idx) const
{
  struct : particle
  {
    std::size_t id_attr_idx = generic_particle::NOT_STORED;
    std::size_t coord_attr_idx = generic_particle::NOT_STORED;
    generic_particle const *self;
    double coord(std::size_t d) const override
    {
      if(generic_particle::NOT_STORED == coord_attr_idx)
        return 0;
      attribute const &attr = self->find_attribute(coord_attr_idx);
      return attr.get_real_ref(d);
    }
    std::size_t id() const override
    {
      if(generic_particle::NOT_STORED == id_attr_idx)
        return 0;
      return self->find_attribute(id_attr_idx).get_integer_ref();
    }
    attribute extra_attribute(std::size_t attrno) const override
    {
      return self->find_attribute(attrno);
    }
  } particle_view;
  particle_view.self = this;
  particle_view.id_attr_idx = id_attr_idx;
  particle_view.coord_attr_idx = coord_attr_idx;

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
