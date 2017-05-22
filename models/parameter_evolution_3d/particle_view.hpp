#ifndef PARTICLE_VIEW_HPP
#define PARTICLE_VIEW_HPP

#include "parameter_evolution_3d_fwd.hpp"

#include "interface/attribute_list.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
struct particle_view : public simbad::core::attribute_list
{
  cell const &m_cell;
  parameter_evolution_3d const &m_model;
  particle_view(cell const &c, parameter_evolution_3d const &model)
      : m_cell(c), m_model(model)
  {
  }
  simbad::core::attribute get_attribute(std::size_t attr_idx) const override;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARTICLE_VIEW_HPP
