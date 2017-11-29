#include "configuration_reader.hpp"

#include "configurations/default_attributed_configuration.hpp"
#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE
void configuration_reader::set_configuration(const configuration_view &conf)
{
  read_configuration(conf);
}

void configuration_reader::set_configuration(
    const configuration_view &conf, const property_tree &default_properties)
{
  read_configuration(
      default_attributed_configuration(conf, default_properties));
}
END_NAMESPACE_CORE
