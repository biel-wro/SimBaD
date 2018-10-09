#include "repositories/factory_registration.hpp"

#include "io/csv_printer.hpp"
#include "io/json_configuration_printer.hpp"
#include "io/null_printer.hpp"
#include "io/text_printer.hpp"

BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(stream_printer)
{
  factory<stream_printer> factory;

  register_creator<csv_printer>(factory, "csv_printer");
  register_creator<null_printer>(factory, "null_printer");
  register_creator<text_printer>(factory, "text_printer");

  return factory;
}

END_NAMESPACE_CORE
