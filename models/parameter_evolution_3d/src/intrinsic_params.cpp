#include "intrinsic_params.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D



float cell_params::death_coef() const { return m_lifespan_coef; }
float &cell_params::death_coef() { return m_lifespan_coef; }
float cell_params::death_susceptibility() const
{
  return m_lifespan_susceptibility;
}
float &cell_params::death_susceptibility() { return m_lifespan_susceptibility; }
float cell_params::birth_coef() const { return m_birth_coef; }
float &cell_params::birth_coef() { return m_birth_coef; }
float cell_params::birth_susceptibility() const
{
  return m_birth_susceptibility;
}
float &cell_params::birth_susceptibility() { return m_birth_susceptibility; }
float cell_params::failure_coef() const { return m_failure_coef; }
float &cell_params::failure_coef() { return m_failure_coef; }
float cell_params::failure_susceptibility() const
{
  return m_failure_susceptibility;
}
float &cell_params::failure_susceptibility()
{
  return m_failure_susceptibility;
}

END_NAMESPACE_PARAMETER_EVOLUTION_3D
