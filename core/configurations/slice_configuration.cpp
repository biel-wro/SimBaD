#include "slice_configuration.hpp"
#include "interface/particle.hpp"
#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE

slice_configuration::slice_configuration(configuration_view const &base,
                                         property_tree const &pt)
    : slice_configuration(base, pt.get<std::size_t>("slicing.dimension"),
                          pt.get<double>("slicing.min"),
                          pt.get<double>("slicing.max"))
{
}

slice_configuration::slice_configuration(configuration_view const &base,
                                         std::size_t slicing_dim,
                                         double slice_min, double slice_max)

    : enriched_configuration(base),
      m_slicing_dimension(slicing_dim),
      m_slice_min(slice_min),
      m_slice_max(slice_max)
{
}

void slice_configuration::visit_configuration(particle_visitor v) const
{
  get_base().visit_configuration([this, v](particle const &p) {
    double slicing_coord = p.coord(m_slicing_dimension);
    if(slicing_coord < m_slice_min || m_slice_max < slicing_coord)
      return;
    v(p);
  });
}

END_NAMESPACE_CORE
