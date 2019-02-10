#include "repositories/factory_registration.hpp"
#include <computational/mutations/stateless_beta.hpp>

#include "computational/mutations/skewed_beta.hpp"
#include "computational/mutations/skewed_polynomial.hpp"
#include "computational/mutations/stateless_beta.hpp"
#include "computational/mutations/uniform_step.hpp"
#include "computational/mutations/void_mutator.hpp"

BEGIN_NAMESPACE_CORE

SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(mutator<double>)
{
  factory<mutator<double>> factory;

  register_creator<skewed_beta<double>>(factory, "skewed_beta");
  register_creator<skewed_polynomial<double>>(factory, "skewed_polynomial");
  register_creator<stateless_beta<double>>(factory, "stateless_beta");
  register_creator<uniform_step<double>>(factory, "uniform_step");
  register_creator<void_mutator<double>>(factory, "void_mutator");

  return factory;
}

END_NAMESPACE_CORE