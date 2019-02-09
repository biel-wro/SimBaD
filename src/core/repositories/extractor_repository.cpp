#include "repositories/factory_registration.hpp"

#include "computational/extractors/templ/complementary_generalized_exponential.hpp"
#include "computational/extractors/templ/generalized_exponential.hpp"
#include "computational/extractors/templ/inverse_generalized_exponential.hpp"

BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(extractor<double>)
{
  factory<extractor<double>> factory;

  register_creator<generalized_exponential<double>>(factory,
      "generalized_exponential");
  register_creator<inverse_generalized_exponential<double>>(
      factory, "inverse_generalized_exponential");
  register_creator<complementary_generalized_exponential<double>>(
      factory, "complementary_generalized_exponential");

  return factory;
}

END_NAMESPACE_CORE
