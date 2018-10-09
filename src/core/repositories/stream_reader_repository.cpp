#include "repositories/factory_registration.hpp"

#include "io/csv_reader.hpp"

BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(stream_reader)
{
  factory<stream_reader> factory;

  register_creator<csv_reader>(factory, "csv_reader");
  return factory;
}
END_NAMESPACE_CORE
