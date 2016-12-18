#include "selected_attributes_configuration.hpp"
#include "core_fwd.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_mapping.hpp"

#include <string>
BEGIN_NAMESPACE_CORE
selected_attributes_configuration::selected_attributes_configuration(
    const configuration_view &base, std::vector<std::string> const &white_list)
    : enriched_configuration(base), m_mapping()
{
  attribute_mapping const &other_mapping = base.attr_map();
  for(std::string const &attr_name : white_list)
    m_mapping.insert(other_mapping[attr_name]);
}

selected_attributes_configuration::selected_attributes_configuration(
    const configuration_view &base, const property_tree &pt)
    : selected_attributes_configuration(
          base, pt.get_vector<std::string>("white_list"))
{
}

const attribute_mapping &selected_attributes_configuration::attr_map() const
{
  return m_mapping;
}
END_NAMESPACE_CORE
