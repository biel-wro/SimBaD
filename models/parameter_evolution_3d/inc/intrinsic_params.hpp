#ifndef INTRINSIC_PARAMS_HPP
#define INTRINSIC_PARAMS_HPP

#include "parameter_evolution_3d_fwd.hpp"

#include "core_fwd.hpp"
#include <random>
BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

struct cell_params
{
public:
  cell_params(simbad::core::property_tree const &pt);
  cell_params(simbad::core::particle const &p,
              std::vector<std::size_t> const &attribute_indexes);
  cell_params(float birth_eff, float birth_res, float lifespan_eff,
              float lifespan_res, float success_eff, float success_res);
  static std::vector<std::size_t> get_attribute_indices(
      simbad::core::attribute_mapping const &mapping);

  float lifespan_eff() const;
  float &lifespan_eff();
  float lifespan_res() const;
  float &lifespan_res();
  float birth_eff() const;
  float &birth_eff();
  float birth_res() const;
  float &birth_res();
  float success_eff() const;
  float &success_eff();
  float success_res() const;
  float &success_res();

private:
  float m_birth_eff;
  float m_birth_res;
  float m_lifespan_eff;
  float m_lifespan_res;
  float m_success_eff;
  float m_success_res;
};

END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // INTRINSIC_PARAMS_HPP
