#include "configurations/default_attributed_configuration.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_list.hpp"
#include "interface/property_tree.hpp"

#include <algorithm>
#include <cmath>
BEGIN_NAMESPACE_CORE
default_attributed_configuration::default_attributed_configuration(
    const configuration_view &base, const property_tree &pt)
    : enriched_configuration(base),
      m_properties(pt),
      m_owned_attributes(),
      m_merged_attr_desc()
{
  on_base_reset();
}

void default_attributed_configuration::on_base_reset()
{
  m_merged_attr_desc.clear();
  m_merged_attr_desc = get_base().descriptor();
  m_owned_attributes = m_merged_attr_desc.add_and_map_attributes(m_properties);
}
void default_attributed_configuration::visit_records(
    configuration_view::particle_visitor v) const
{
  get_base().visit_records([this, v](attribute_list const &al) {
    struct : public attribute_list
    {
      attribute_list const *m_base;
      default_attributed_configuration const *m_config;
      attribute get_attribute(std::size_t attr_idx) const override
      {
        default_attributed_configuration::attribute_map::const_iterator it;
        it = m_config->m_owned_attributes.find(attr_idx);
        if(m_config->m_owned_attributes.end() == it)
          return (*m_base)[attr_idx];
        return it->second;
      }
    } new_attributes;
    new_attributes.m_base = &al;
    new_attributes.m_config = this;
    v(new_attributes);
  });
}

const attribute_description &
default_attributed_configuration::descriptor() const
{
  return m_merged_attr_desc;
}

END_NAMESPACE_CORE
