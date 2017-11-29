#include "selected_attributes_configuration.hpp"
#include "core_fwd.hpp"

#include "interface/attribute_description.hpp"
#include "interface/property_tree.hpp"

#include <string>
BEGIN_NAMESPACE_CORE
selected_attributes_configuration::selected_attributes_configuration(
    const configuration_view &base, std::vector<std::string> const &white_list)
    : enriched_configuration(base), m_mapping()
{
  attribute_description const &other_mapping = base.descriptor();
  for(std::string const &attr_name : white_list)
    m_mapping.insert(other_mapping[attr_name]);
}

selected_attributes_configuration::selected_attributes_configuration(
    const configuration_view &base, const property_tree &pt)
    : selected_attributes_configuration(
          base, pt.get_vector<std::string>("white_list"))
{
}

const attribute_description &
selected_attributes_configuration::descriptor() const
{
  return m_mapping;
}
END_NAMESPACE_CORE
