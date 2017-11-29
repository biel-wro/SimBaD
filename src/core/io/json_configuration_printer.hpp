#ifndef JSON_CONFIGURATION_PRINTER_HPP
#define JSON_CONFIGURATION_PRINTER_HPP

#include "core_fwd.hpp"
#include "interface/stream_printer.hpp"

BEGIN_NAMESPACE_CORE
class json_configuration_printer : stream_printer
{
public:
  json_configuration_printer(std::ostream &ostream);
};

END_NAMESPACE_CORE
#endif // TEXT_CONFIGURATION_PRINTER_HPP
