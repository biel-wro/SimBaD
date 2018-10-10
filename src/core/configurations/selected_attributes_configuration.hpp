#ifndef SELECTED_ATTRIBUTES_CONFIGURATION_HPP
#define SELECTED_ATTRIBUTES_CONFIGURATION_HPP
#include "configurations/enriched_configuration.hpp"
#include "interface/attribute_description.hpp"
#include "interface/configuration_view.hpp"
#include "interface/interface_fwd.hpp"

BEGIN_NAMESPACE_CORE

class selected_attributes_configuration final : public enriched_configuration
{
public:
  selected_attributes_configuration(configuration_view const &base,
                                    std::vector<std::string> const &white_list);
  selected_attributes_configuration(configuration_view const &base,
                                    property_tree const &pt);

  attribute_description const &description() const;

private:
  attribute_description m_mapping;
};
END_NAMESPACE_CORE

#endif // SELECTED_ATTRIBUTES_CONFIGURATION_HPP
