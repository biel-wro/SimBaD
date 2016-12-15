#include "configuration_reader.hpp"

#include "configurations/enriched_configuration.hpp"
#include "interface/attribute.hpp"
#include "attribute_descriptor.hpp"
#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE
void configuration_reader::set_configuration(const configuration_view &conf)
{
  property_tree empty_tree;
  read_configuration(conf);
}

void configuration_reader::set_configuration(
    const configuration_view &conf, const property_tree &default_properties)
{
  read_configuration(enriched_configuration(conf, default_properties));
}
END_NAMESPACE_CORE
