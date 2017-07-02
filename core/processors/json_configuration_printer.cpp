#include "json_configuration_printer.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/configuration_view.hpp"
#include "interface/property_tree.hpp"
#include <cstddef>
#include <iostream>

BEGIN_NAMESPACE_CORE

json_configuration_printer::json_configuration_printer(std::ostream &ostream)
    : stream_printer(ostream)
{
}

END_NAMESPACE_CORE
