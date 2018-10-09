#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include "parameter_evolution_3d_fwd.hpp"

#include "computational/computational_fwd.hpp"
#include "computational/dispersions/gaussian_dispersion.hpp"
#include "computational/extractors/generalized_exponential_extractor.hpp"
#include "computational/interactions/generalized_exponential_interaction.hpp"
#include "computational/mutations/mutator.hpp"

#include "interface/property_tree.hpp"

#include <functional>
#include <random>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
struct model_params
{
  using interaction_type = core::generalized_exponential_interaction<double>;
  using dispersion_type = core::gaussian_dispersion<double>;
  using mutator_ptr = std::unique_ptr<core::mutator>;
  using time_scalar_ptr = std::unique_ptr<core::time_dependent_scalar>;

  model_params(simbad::core::property_tree const &pt);
  ~model_params();

  double birth_rate(double density, double efficiency, double resistance) const;
  double lifespan(double density, double efficiency, double resistance) const;
  double success_prob(double density, double efficiency, double resistan) const;

  bool sample_mutation(cell &cp, std::mt19937_64 &rng) const;
  void mutate_birth(cell_params &cp, std::mt19937_64 &rnd) const;
  void mutate_lifespan(cell_params &cp, std::mt19937_64 &rnd) const;
  void mutate_success(cell_params &cp, std::mt19937_64 &rnd) const;
  interaction_type const &interaction() const;
  dispersion_type const &dispersion() const;

protected:
  double birth_saturation(double x) const;
  double lifespan_saturation(double x) const;
  double success_saturation(double x) const;

private:
  double m_mutation_prob;

  interaction_type m_interaction;
  dispersion_type m_dispersion;
  using extractor_type =
      simbad::core::generalized_exponential_extractor<double>;
  extractor_type m_birth_extractor;
  extractor_type m_lifespan_extractor;
  extractor_type m_success_extractor;

  mutator_ptr const m_birth_eff_mutator_ptr;
  mutator_ptr const m_birth_res_mutator_ptr;
  mutator_ptr const m_lifespan_eff_mutator_ptr;
  mutator_ptr const m_lifespan_res_mutator_ptr;
  mutator_ptr const m_success_eff_mutator_ptr;
  mutator_ptr const m_success_res_mutator_ptr;

  time_scalar_ptr const m_success_eff_time_dep;
  time_scalar_ptr const m_success_res_time_dep;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARAMETERS_HPP
