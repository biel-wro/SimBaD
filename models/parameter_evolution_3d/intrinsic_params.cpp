#include "intrinsic_params.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/configuration_view.hpp"
#include "interface/particle.hpp"
#include "interface/property_tree.hpp"
#include "utils/attribute_converter.hpp"

#include <limits>
BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

cell_params::cell_params(const simbad::core::property_tree &pt)
    : cell_params(pt.get<float>("birth.efficiency"),
                  pt.get<float>("birth.resistance"),
                  pt.get<float>("lifespan.efficiency"),
                  pt.get<float>("lifespan.resistance"),
                  pt.get<float>("success.efficiency"),
                  pt.get<float>("success.resistance"))
{
}

cell_params::cell_params(const core::attribute_list &p,
                         std::vector<std::size_t> const &attribute_indices)
    : cell_params(p[attribute_indices[0]].get_real_val(),
                  p[attribute_indices[1]].get_real_val(),
                  p[attribute_indices[2]].get_real_val(),
                  p[attribute_indices[3]].get_real_val(),
                  p[attribute_indices[4]].get_real_val(),
                  p[attribute_indices[5]].get_real_val())
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
    simbad::core::attribute_descriptor const &mapping)
{
  return {mapping["birth.efficiency"].attribute_idx(),
          mapping["birth.resistance"].attribute_idx(),
          mapping["lifespan.efficiency"].attribute_idx(),
          mapping["lifespan.resistance"].attribute_idx(),
          mapping["success.efficiency"].attribute_idx(),
          mapping["success.resistance"].attribute_idx()};
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
