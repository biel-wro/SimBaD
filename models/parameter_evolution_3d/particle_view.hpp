#ifndef PARTICLE_VIEW_HPP
#define PARTICLE_VIEW_HPP

#include "parameter_evolution_3d_fwd.hpp"
#include "interface/attribute_list.hpp"

//

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
struct particle_view : public simbad::core::attribute_list
{
  particle_view(cell const &c, parameter_evolution_3d const &model);
  cell const &get_cell() const { return m_cell; }
  parameter_evolution_3d const &get_model() const { return m_model; }
  simbad::core::attribute get_attribute(std::size_t attr_idx) const override;

private:
  cell const &m_cell;
  parameter_evolution_3d const &m_model;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARTICLE_VIEW_HPP
