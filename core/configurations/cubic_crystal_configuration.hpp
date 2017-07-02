#ifndef CUBIC_CRYSTAL_CONFIGURATION_HPP
#define CUBIC_CRYSTAL_CONFIGURATION_HPP

#include "interface/configuration_view.hpp"
#include <memory>
BEGIN_NAMESPACE_CORE

class cubic_crystal_configuration final : public configuration_view
{
public:
  explicit cubic_crystal_configuration(property_tree const &pt);
  explicit cubic_crystal_configuration(size_type dimension, double radius,
                                       double spacing);
  size_type size() const override;
  void visit_records(particle_visitor) const override;
  attribute_description const &descriptor() const override;
  ~cubic_crystal_configuration();

protected:
  size_type spacings_per_radius() const;

private:
  size_type m_dimension;
  double m_radius;
  double m_spacing;
  std::unique_ptr<mapping> m_attr_mapping_ptr;
};
END_NAMESPACE_CORE
#endif // CUBIC_CRYSTAL_CONFIGURATION_HPP
