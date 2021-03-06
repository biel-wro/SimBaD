#include "configuration_view.hpp"
#include "interface/attribute_description.hpp"
#include <boost/optional.hpp>
BEGIN_NAMESPACE_CORE

std::size_t configuration_view::position_attr_idx() const
{
  boost::optional<attribute_descriptor const &> desc_opt =
      description().get_descriptor(ATTRIBUTE_KIND::PARTICLE_POSITION);
  assert(desc_opt);
  return desc_opt.get().attribute_idx();
}

configuration_view::size_type configuration_view::dimension() const
{
  boost::optional<attribute_descriptor const &> desc_opt =
      description().get_descriptor(ATTRIBUTE_KIND::PARTICLE_POSITION);
  assert(desc_opt);
  return desc_opt.get().attribute_dimension();
}

bool configuration_view::has_unique_id() const
{
  boost::optional<attribute_descriptor const &> desc_opt =
      description().get_descriptor(ATTRIBUTE_KIND::PARTICLE_POSITION);
  return desc_opt.is_initialized();
}

configuration_view::~configuration_view() = default;
END_NAMESPACE_CORE
