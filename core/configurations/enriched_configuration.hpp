#ifndef ENRICHED_CONFIGURATION_HPP
#define ENRICHED_CONFIGURATION_HPP

#include "core_fwd.hpp"
#include "interface/configuration_view.hpp"
#include "interface/property_tree.hpp"
#include "interface/attribute_mapping.hpp"

#include <unordered_map>
#include <vector>

BEGIN_NAMESPACE_CORE

class enriched_configuration : public configuration_view
{
public:
  using attribute_map = std::unordered_map<std::size_t, std::string>;
  explicit enriched_configuration(
      configuration_view const &m_base,
      property_tree const &attributes);
  size_type configuration_size() const override;
  size_type dimension() const override;
  void visit_configuration(particle_visitor) const override;
  //attribute_range attribute_descriptors() const override;
  attribute_mapping const &attr_map() const override;
protected:
  void static store_extra_attributes(std::vector<attribute> &attributes,
                                     property_tree const &pt,
                                     std::string prefix = "");

private:
  configuration_view const &m_base;
  attribute_mapping m_attribute_mapping;
  //std::vector<attribute_descriptor> m_descriptors;
  attribute_map m_new_attributes;
};
END_NAMESPACE_CORE

#endif // ENRICHED_CONFIGURATION_HPP
