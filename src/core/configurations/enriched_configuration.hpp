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
  virtual void on_base_reset();
  size_type size() const override;
  void visit_records(particle_visitor) const override;
  attribute_description const &description() const override;

private:
  configuration_view const *m_base;
};
END_NAMESPACE_CORE

#endif // ENRICHED_CONFIGURATION_HPP
