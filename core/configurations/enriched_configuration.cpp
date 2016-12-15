#include "enriched_configuration.hpp"

#include "configurations/cubic_crystal_configuration.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/particle.hpp"
#include "interface/property_tree.hpp"

#include <algorithm>
#include <cmath>
BEGIN_NAMESPACE_CORE
enriched_configuration::enriched_configuration(const configuration_view &base,
                                               const property_tree &pt)
    : m_base(base),
      m_attribute_mapping(),
      m_new_attributes(m_attribute_mapping.add_attributes(pt))
{
}

configuration_view::size_type enriched_configuration::configuration_size() const
{
  return m_base.configuration_size();
}

configuration_view::size_type enriched_configuration::dimension() const
{
  return m_base.dimension();
}

void enriched_configuration::visit_configuration(particle_visitor v) const
{
  m_base.visit_configuration([this, v](particle const &p) {
    struct : public particle
    {
      particle const *m_base;
      enriched_configuration const *m_config;
      double coord(std::size_t d) const override { return m_base->coord(d); }
      std::size_t id() const override { return m_base->id(); }
    protected:
      attribute extra_attribute(std::size_t attrno) const override
      {
        enriched_configuration::attribute_map::const_iterator it;
        it = m_config->m_new_attributes.find(attrno);
        if(m_config->m_new_attributes.end() == it)
          return m_base->get_attribute(attrno);
        return it->second;
      }
    } new_view;
    new_view.m_base = &p;
    new_view.m_config = this;
    v(new_view);
  });
}
/*
attribute_range enriched_configuration::attribute_descriptors() const
{
  return attribute_range(m_descriptors.begin(), m_descriptors.end());
}*/

const attribute_mapping &enriched_configuration::attr_map() const
{
  return m_attribute_mapping;
}
void enriched_configuration::store_extra_attributes(
    std::vector<attribute> &attributes, property_tree const &pt,
    std::string prefix)
{
  throw "!!!";
}

END_NAMESPACE_CORE
