#include "repositories/factory_registration.hpp"

#include "processors/advancers/in_simulation_time_advancer.hpp"
#include "processors/advancers/num_events_advancer.hpp"
#include "processors/advancers/population_size_advancer.hpp"
BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECLARATION(advancer)
{
  factory<advancer> f;

  register_creator<in_simulation_time_advancer>(f,
                                                "in_simulation_time_advancer");
  register_creator<num_events_advancer>(f, "num_events_advancer");
  register_creator<population_size_advancer>(f, "population_size_advancer");

  return f;
}
END_NAMESPACE_CORE
