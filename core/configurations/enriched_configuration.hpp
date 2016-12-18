#ifndef ENRICHED_CONFIGURATION_HPP
#define ENRICHED_CONFIGURATION_HPP

#include "interface/configuration_view.hpp"

BEGIN_NAMESPACE_CORE
class enriched_configuration : public configuration_view
{
public:
  explicit enriched_configuration(configuration_view const &m_base);

  virtual void set_base(configuration_view const &base) final;
  virtual configuration_view const &get_base() const final;
  size_type configuration_size() const override;
  size_type dimension() const override;
  void visit_configuration(particle_visitor) const override;
  attribute_mapping const &attr_map() const override;

private:
  configuration_view const *m_base;
};
END_NAMESPACE_CORE

#endif // ENRICHED_CONFIGURATION_HPP
