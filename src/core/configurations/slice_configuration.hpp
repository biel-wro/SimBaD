#ifndef FILTER_CONFIGURATION_HPP
#define FILTER_CONFIGURATION_HPP

#include "configurations/enriched_configuration.hpp"
#include "interface/interface_fwd.hpp"
BEGIN_NAMESPACE_CORE

class slice_configuration final : public enriched_configuration
{
public:
  slice_configuration(configuration_view const &base, property_tree const &pt);
  slice_configuration(configuration_view const &base, std::size_t slicing_dim,
                      double slice_min, double slice_max);
  void visit_records(particle_visitor v) const override;

private:
  std::size_t m_slicing_dimension;
  double m_slice_min;
  double m_slice_max;
};
END_NAMESPACE_CORE
#endif // FILTER_CONFIGURATION_HPP
