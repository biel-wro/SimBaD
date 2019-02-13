#include "repositories/factory_registration.hpp"

#include "identity.hpp"

BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(time_dependent_scalar)
{
  factory<time_dependent_scalar> factory;
  register_creator<identity>(factory, "identity");
  return factory;
}

END_NAMESPACE_CORE
