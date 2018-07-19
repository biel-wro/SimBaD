#include "repositories/repository_registration.hpp"

#include "io/csv_printer.hpp"
#include "io/json_configuration_printer.hpp"
#include "io/null_printer.hpp"
#include "io/text_printer.hpp"

BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECLARATION(stream_printer)
{
  repository<stream_printer> repository;

  register_creator<csv_printer>(repository, "csv_printer");
  register_creator<null_printer>(repository, "null_printer");
  register_creator<text_printer>(repository, "text_printer");

  return repository;
}

END_NAMESPACE_CORE