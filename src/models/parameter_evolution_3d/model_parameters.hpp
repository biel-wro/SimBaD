#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include "parameter_evolution_3d_fwd.hpp"

#include "computational/computational_fwd.hpp"
#include "computational/dispersions/gaussian_dispersion.hpp"
#include "computational/extractors/templ/generalized_exponential.hpp"
#include "computational/interactions/generalized_exponential_interaction.hpp"
#include "interface/mutator.hpp"

#include "interface/property_tree.hpp"

#include <functional>
#include <random>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
struct model_params
{
  using interaction_type = core::generalized_exponential_interaction<double>;
  using dispersion_type = core::gaussian_dispersion<double>;
  using mutator_ptr = std::unique_ptr<core::mutator<double>>;
  using extractor_ptr = std::unique_ptr<core::extractor<double>>;
  using time_scalar_ptr = std::unique_ptr<core::time_dependent_scalar>;

  explicit model_params(simbad::core::property_tree const &pt);
  ~model_params();

  double birth_rate(double density, double efficiency, double resistance) const;
  double death_rate(double density, double efficiency, double resistance) const;
  double success_prob(double densi, double efficiency, double resistance) const;

  bool sample_mutation(cell &cp, std::mt19937_64 &rng) const;
  void mutate_birth(cell_params &cp, std::mt19937_64 &rnd) const;
  void mutate_death(cell_params &cp, std::mt19937_64 &rnd) const;
  void mutate_success(cell_params &cp, std::mt19937_64 &rnd) const;
  interaction_type const &interaction() const;
  dispersion_type const &dispersion() const;
protected:

private:
  double m_mutation_prob;

  interaction_type m_interaction;
  dispersion_type m_dispersion;

  extractor_ptr const m_birth_extractor_ptr;
  extractor_ptr const m_death_extractor_ptr;
  extractor_ptr const m_success_extractor_ptr;

  mutator_ptr const m_birth_eff_mutator_ptr;
  mutator_ptr const m_birth_res_mutator_ptr;
  mutator_ptr const m_death_eff_mutator_ptr;
  mutator_ptr const m_death_res_mutator_ptr;
  mutator_ptr const m_success_eff_mutator_ptr;
  mutator_ptr const m_success_res_mutator_ptr;

  time_scalar_ptr const m_success_eff_time_dep;
  time_scalar_ptr const m_success_res_time_dep;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARAMETERS_HPP
