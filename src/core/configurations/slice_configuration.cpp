#include "slice_configuration.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
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

void slice_configuration::visit_records(
    configuration_view::particle_visitor v) const
{
  attribute_description const &map = get_base().description();
  assert(map.get_descriptor(ATTRIBUTE_KIND::PARTICLE_POSITION));
  attribute_descriptor d =
      map.get_descriptor(ATTRIBUTE_KIND::PARTICLE_POSITION).get();
  std::size_t attr_idx = d.attribute_idx();

  get_base().visit_records([=](attribute_list const &al) {
    double slicing_coord = al[attr_idx].get_real_ref(m_slicing_dimension);
    if(slicing_coord < m_slice_min || m_slice_max < slicing_coord)
      return;
    v(al);
  });
}

END_NAMESPACE_CORE
