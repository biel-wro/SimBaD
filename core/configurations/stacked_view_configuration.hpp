#ifndef STACKED_VIEW_CONFIGURATION_HPP
#define STACKED_VIEW_CONFIGURATION_HPP

#include "configurations/enriched_configuration.hpp"
#include "core_def.hpp"

#include <list>

BEGIN_NAMESPACE_CORE
class stacked_view_configuration final: public enriched_configuration
{
public:
  stacked_view_configuration(configuration_view const &base);
  stacked_view_configuration(configuration_view const &base,
                             property_tree const &pt);
  configuration_view const &last_layer() const;

  size_type configuration_size() const override;
  void visit_configuration(particle_visitor visitor) const override;

  size_type dimension() const override;
  bool has_unique_id() const override;
  attribute_mapping const &attr_map() const override;

  void push_layer(std::unique_ptr<enriched_configuration> layer);
  void push_layer(std::string const &layer_name, property_tree const &pt);
  std::unique_ptr<enriched_configuration> pop_layer();

private:
  std::list<std::unique_ptr<enriched_configuration>> m_layers;
};

END_NAMESPACE_CORE
#endif // STACKED_VIEW_CONFIGURATION_HPP
