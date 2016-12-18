#include "configurations/default_attributed_configuration.hpp"

#include "configurations/cubic_crystal_configuration.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/particle.hpp"
#include "interface/property_tree.hpp"

#include <algorithm>
#include <cmath>
BEGIN_NAMESPACE_CORE
default_attributed_configuration::default_attributed_configuration(
    const configuration_view &base, const property_tree &pt)
    : enriched_configuration(base),
      m_attribute_mapping(),
      m_new_attributes(m_attribute_mapping.add_attributes(pt))
{
}


void default_attributed_configuration::visit_configuration(
    particle_visitor v) const
{
  get_base().visit_configuration([this, v](particle const &p) {
    struct : public particle
    {
      particle const *m_base;
      default_attributed_configuration const *m_config;
      double coord(std::size_t d) const override { return m_base->coord(d); }
      std::size_t id() const override { return m_base->id(); }
    protected:
      attribute extra_attribute(std::size_t attrno) const override
      {
        default_attributed_configuration::attribute_map::const_iterator it;
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

const attribute_mapping &default_attributed_configuration::attr_map() const
{
  return m_attribute_mapping;
}
void default_attributed_configuration::store_extra_attributes(
    std::vector<attribute> &attributes, property_tree const &pt,
    std::string prefix)
{
  throw "!!!";
}

END_NAMESPACE_CORE
