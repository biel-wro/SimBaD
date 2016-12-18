#ifndef CUBIC_CRYSTAL_CONFIGURATION_HPP
#define CUBIC_CRYSTAL_CONFIGURATION_HPP

#include "interface/configuration_view.hpp"

BEGIN_NAMESPACE_CORE

class cubic_crystal_configuration : public configuration_view
{
public:
  explicit cubic_crystal_configuration(property_tree const &pt);
  explicit cubic_crystal_configuration(size_type dimension, double radius,
                                       double spacing);
  size_type configuration_size() const override;
  size_type dimension() const override;
  void visit_configuration(particle_visitor) const override;

protected:
  size_type spacings_per_radius() const;

private:
  size_type m_dimension;
  double m_radius;
  double m_spacing;
};
END_NAMESPACE_CORE
#endif // CUBIC_CRYSTAL_CONFIGURATION_HPP
