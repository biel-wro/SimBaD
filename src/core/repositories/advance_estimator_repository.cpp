#include "repositories/factory_registration.hpp"

#include "processors/advance_estimators/in_simulation_time.hpp"
#include "processors/advance_estimators/num_events.hpp"
#include "processors/advance_estimators/population_size.hpp"
BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(advance_estimator)
{
  factory<advance_estimator> f;

  namespace ae = advance_esimators;

  register_creator<ae::in_simulation_time>(f, "in_simulation_time");
  register_creator<ae::num_events>(f, "num_events");
  register_creator<ae::population_size>(f, "population_size");

  return f;
}
END_NAMESPACE_CORE
