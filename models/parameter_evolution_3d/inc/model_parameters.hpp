#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP
#include "parameter_evolution_3d_fwd.hpp"

#include "extractors/generalized_exponential_extractor.hpp"
#include "interactions/generalized_exponential_interaction.hpp"
#include "property_tree.hpp"
#include <random>
BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
struct model_params
{
  model_params(simbad::core::property_tree const &pt);
  ~model_params();

  double birth_rate(double density, double efficiency, double suscept) const;
  double lifetime(double density, double efficiency, double suscept) const;
  double failure_prob(double density, double efficiency, double suscept) const;

  void mutate(cell_params &cp, std::mt19937_64 &rnd) const;

protected:
  double birth_saturation(double x) const;
  double lifetime_saturation(double x) const;
  double failure_saturation(double x) const;

  void mutate_birth(cell_params &cp, std::mt19937_64 &rnd) const;
  void mutate_death(cell_params &cp, std::mt19937_64 &rnd) const;
  void mutate_failure(cell_params &cp, std::mt19937_64 &rnd) const;

private:
  using interaction = simbad::core::generalized_exponential_interaction<double>;
  interaction m_interaction;
  using extractor = simbad::core::generalized_exponential_extractor<double>;
  extractor m_birth_extractor;
  extractor m_lifetime_extractor;
  extractor m_failure_extractor;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARAMETERS_HPP
