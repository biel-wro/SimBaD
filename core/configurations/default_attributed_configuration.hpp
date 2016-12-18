#ifndef ATTRIBUTE_DEFAULTED_CONFIGURATION_HPP
#define ATTRIBUTE_DEFAULTED_CONFIGURATION_HPP

#include "core_fwd.hpp"
#include "configurations/enriched_configuration.hpp"
#include "interface/attribute_mapping.hpp"

#include <unordered_map>
#include <vector>

BEGIN_NAMESPACE_CORE

class default_attributed_configuration final : public enriched_configuration
{
public:
  using attribute_map = std::unordered_map<std::size_t, std::string>;
  explicit default_attributed_configuration(
      configuration_view const &m_base,
      property_tree const &attributes);

  void visit_configuration(particle_visitor) const override;
  attribute_mapping const &attr_map() const override;
protected:
  void static store_extra_attributes(std::vector<attribute> &attributes,
                                     property_tree const &pt,
                                     std::string prefix = "");

private:
  attribute_mapping m_attribute_mapping;
  attribute_map m_new_attributes;
};
END_NAMESPACE_CORE

#endif // ATTRIBUTE_DEFAULTED_CONFIGURATION_HPP
