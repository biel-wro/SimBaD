#ifndef INTRINSIC_PARAMS_HPP
#define INTRINSIC_PARAMS_HPP

#include "parameter_evolution_3d_fwd.hpp"
#include <random>
BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

struct cell_params
{
public:
  float death_coef() const;
  float &death_coef();
  float death_susceptibility() const;
  float &death_susceptibility();
  float birth_coef() const;
  float &birth_coef();
  float birth_susceptibility() const;
  float &birth_susceptibility();
  float failure_coef() const;
  float &failure_coef();
  float failure_susceptibility() const;
  float &failure_susceptibility();

private:
  float m_lifespan_coef;
  float m_lifespan_susceptibility;
  float m_birth_coef;
  float m_birth_susceptibility;
  float m_failure_coef;
  float m_failure_susceptibility;
};

END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // INTRINSIC_PARAMS_HPP
