#ifndef ATTRIBUTE_DEFAULTED_CONFIGURATION_HPP
#define ATTRIBUTE_DEFAULTED_CONFIGURATION_HPP

#include "configurations/enriched_configuration.hpp"
#include "interface/interface_fwd.hpp"
#include "interface/attribute_description.hpp"
#include "interface/property_tree.hpp"

#include <unordered_map>
#include <vector>

BEGIN_NAMESPACE_CORE

class default_attributed_configuration final : public enriched_configuration
{
public:
  using attribute_map = std::unordered_map<std::size_t, std::string>;
  explicit default_attributed_configuration(configuration_view const &m_base,
                                            property_tree const &attributes);
  virtual void on_base_reset() override;
  void visit_records(particle_visitor visitor) const override;
  attribute_description const &description() const override;

private:
  property_tree m_properties;
  attribute_map m_owned_attributes;
  attribute_description m_merged_attr_desc;
};
END_NAMESPACE_CORE

#endif // ATTRIBUTE_DEFAULTED_CONFIGURATION_HPP
