#include "particle_view.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_list.hpp"
#include "parameter_evolution_3d.hpp"
#include "utils/attribute_converter.hpp"
#include "utils/attribute_exceptions.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

particle_view::particle_view(const cell &c, const parameter_evolution_3d &model)
    : m_cell(c), m_model(model)
{
}

simbad::core::attribute particle_view::get_attribute(std::size_t attr_idx) const
{
  switch(attr_idx)
  {
  case 0:
    return simbad::core::attribute_converter::convert_to<
               simbad::core::coordinates<double, 3>>(m_cell.position())
        .get();
  case 1: return m_cell.density();
  case 2: return m_cell.event_time();
  case 3: return std::int64_t(m_cell.event_kind());
  case 4: return m_cell.params().birth_eff();
  case 5: return m_cell.params().birth_res();
  case 6: return m_cell.params().lifespan_eff();
  case 7: return m_cell.params().lifespan_res();
  case 8: return m_cell.params().success_eff();
  case 9: return m_cell.params().success_res();
  case 10: return m_model.compute_birth_rate(m_cell);
  case 11: return m_model.compute_death_rate(m_cell);
  case 12: return m_model.compute_success_rate(m_cell);
  case 13: return 1.0 / m_model.compute_death_rate(m_cell);
  case 14: return m_cell.params().dummy();
  }
  throw simbad::core::unrecognized_attribute_number(attr_idx);
  // return m_model.new_attribute(m_cell, idx);
}
END_NAMESPACE_PARAMETER_EVOLUTION_3D
