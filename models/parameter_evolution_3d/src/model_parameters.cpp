#include "model_parameters.hpp"
BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
model_params::model_params(const simbad::core::property_tree &pt)
    : m_interaction(pt.get_child("interaction")),
      m_birth_extractor(pt.get_child("birth.saturation")),
      m_lifetime_extractor(pt.get_child("lifetime.saturation")),
      m_failure_extractor(pt.get_child("failure.saturation"))
{
}
model_params::~model_params() {}
double model_params::birth_rate(double density, double eff,
                                double suscept) const
{
  double val = eff * birth_saturation(density * suscept);
  return val;
}

double model_params::lifetime(double density, double eff, double suscept) const
{
  double val = eff * lifetime_saturation(density * suscept);
  return val;
}

double model_params::failure_prob(double density, double eff,
                                  double suscept) const
{
  double val = eff * failure_saturation(density * suscept);
  return val;
}

void model_params::mutate(cell_params &cp, std::mt19937_64 &rnd) const
{
  mutate_birth(cp, rnd);
  mutate_death(cp, rnd);
  mutate_failure(cp, rnd);
}

double model_params::birth_saturation(double x) const
{
  return m_birth_extractor(x);
}
double model_params::lifetime_saturation(double x) const
{
  return m_lifetime_extractor(x);
}
double model_params::failure_saturation(double x) const
{
  return m_failure_extractor(x);
}

void model_params::mutate_birth(cell_params &cp, std::mt19937_64 &rnd) const
{
  NOT_IMPLEMENTED_YET
}
void model_params::mutate_death(cell_params &cp, std::mt19937_64 &rnd) const
{
  NOT_IMPLEMENTED_YET
}

void model_params::mutate_failure(cell_params &cp, std::mt19937_64 &rnd) const
{
  NOT_IMPLEMENTED_YET
}
END_NAMESPACE_PARAMETER_EVOLUTION_3D
