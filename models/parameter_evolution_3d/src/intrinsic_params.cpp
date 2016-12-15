#include "intrinsic_params.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_mapping.hpp"
#include "interface/configuration_view.hpp"
#include "interface/particle.hpp"
#include "interface/property_tree.hpp"
#include "utils/attribute_converter.hpp"

#include <limits>
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
static double get_attribute(std::size_t d, const simbad::core::particle &p)
{
  simbad::core::attribute const &attr = p.get_attribute(d);
  return simbad::core::attribute_cast<double>(attr);
}

cell_params::cell_params(const simbad::core::particle &p,
                         std::vector<std::size_t> const &attribute_indexes)
    : m_birth_eff(get_attribute(attribute_indexes[0], p)),
      m_birth_res(get_attribute(attribute_indexes[1], p)),
      m_lifespan_eff(get_attribute(attribute_indexes[2], p)),
      m_lifespan_res(get_attribute(attribute_indexes[3], p)),
      m_success_eff(get_attribute(attribute_indexes[4], p)),
      m_success_res(get_attribute(attribute_indexes[5], p))
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

std::vector<std::size_t> cell_params::get_attribute_indices(
    const simbad::core::attribute_mapping &mapping)
{
  std::vector<std::size_t> indices(6);
  indices[0] = mapping["birth.efficiency"].attribute_idx();
  indices[1] = mapping["birth.resistance"].attribute_idx();
  indices[2] = mapping["lifespan.efficiency"].attribute_idx();
  indices[3] = mapping["lifespan.resistance"].attribute_idx();
  indices[4] = mapping["success.efficiency"].attribute_idx();
  indices[5] = mapping["success.resistance"].attribute_idx();
  return indices;
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
