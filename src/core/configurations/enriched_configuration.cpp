#include "enriched_configuration.hpp"

#include "configurations/cubic_crystal_configuration.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/property_tree.hpp"

#include <algorithm>
#include <cmath>
BEGIN_NAMESPACE_CORE
enriched_configuration::enriched_configuration(const configuration_view &base)
    : m_base(&base)
{
}

void enriched_configuration::set_base(const configuration_view &base)
{
  m_base = &base;
  on_base_reset();
}

const configuration_view &enriched_configuration::get_base() const
{
  return *m_base;
}

void enriched_configuration::on_base_reset() {}
configuration_view::size_type enriched_configuration::size() const
{
  return get_base().size();
}

void enriched_configuration::visit_records(particle_visitor v) const
{
  get_base().visit_records(v);
}
const attribute_description &enriched_configuration::description() const
{
  return get_base().description();
}
END_NAMESPACE_CORE
