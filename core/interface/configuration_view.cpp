#include "configuration_view.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_mapping.hpp"
#include "interface/particle.hpp"
BEGIN_NAMESPACE_CORE
bool configuration_view::has_unique_id() const { return false; }
const attribute_mapping &configuration_view::attr_map() const
{
  static attribute_mapping mapping;
  return mapping;
}
configuration_view::~configuration_view() {}
END_NAMESPACE_CORE
