#include "intrinsic_params.hpp"
#include "property_tree.hpp"
BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

cell_params::cell_params(const simbad::core::property_tree &pt)
    : m_birth_eff(pt.get<float>("birth.efficiency")),
      m_birth_res(pt.get<float>("birth.resistance")),
      m_lifespan_eff(pt.get<float>("lifespan.efficiency")),
      m_lifespan_res(pt.get<float>("lifespan.resistance")),
      m_success_eff(pt.get<float>("success.efficiency")),
      m_success_res(pt.get<float>("success.resistance"))
{
}
cell_params::cell_params(float birth_eff, float birth_res, float lifespan_eff,
                         float lifespan_res, float success_eff,
                         float success_res)
    : m_birth_eff(birth_eff),
      m_birth_res(birth_res),
      m_lifespan_eff(lifespan_eff),
      m_lifespan_res(lifespan_res),
      m_success_eff(success_eff),
      m_success_res(success_res)
{
}

float cell_params::lifespan_eff() const { return m_lifespan_eff; }
float &cell_params::lifespan_eff() { return m_lifespan_eff; }
float cell_params::lifespan_res() const { return m_lifespan_res; }
float &cell_params::lifespan_res() { return m_lifespan_res; }
float cell_params::birth_eff() const { return m_birth_eff; }
float &cell_params::birth_eff() { return m_birth_eff; }
float cell_params::birth_res() const { return m_birth_res; }
float &cell_params::birth_res() { return m_birth_res; }
float cell_params::success_eff() const { return m_success_eff; }
float &cell_params::success_eff() { return m_success_eff; }
float cell_params::success_res() const { return m_success_res; }
float &cell_params::success_res() { return m_success_res; }
END_NAMESPACE_PARAMETER_EVOLUTION_3D
