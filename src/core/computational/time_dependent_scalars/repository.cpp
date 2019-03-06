#include "repositories/factory_registration.hpp"

#include "identity.hpp"
#include "segment_multiplied.hpp"
#include "teeth.hpp"

BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(time_dependent_scalar)
{
  factory<time_dependent_scalar> factory;
  register_creator<identity>(factory, "identity");
  register_creator<segment_multiplied>(factory, "segment_multiplied");
  register_creator<teeth>(factory, "teeth");
  return factory;
}

END_NAMESPACE_CORE
