#include "repositories/factory_registration.hpp"

#include "configurations/cubic_crystal_configuration.hpp"
#include "configurations/poisson_configuration.hpp"

BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(configuration_view)
{
  factory<configuration_view> f;

  register_creator<cubic_crystal_configuration>(f, "cubic_crystal");
  register_creator<poisson_configuration>(f, "poisson");
  return f;
}
END_NAMESPACE_CORE
