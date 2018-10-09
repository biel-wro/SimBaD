#include "repositories/factory_registration.hpp"

#include "constant_scalar.hpp"

BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(time_dependent_scalar)
{
  factory<time_dependent_scalar> factory;

  register_creator<constant_scalar>(factory, "constant_scalar");

  return factory;
}

END_NAMESPACE_CORE
