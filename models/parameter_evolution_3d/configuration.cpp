#include "configuration.hpp"
BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

namespace{
struct particle_attr : public simbad::core::attribute_list
{
  cell const &m_cell;
  parameter_evolution_3d const &m_model;
  particle_attr(cell const &c, parameter_evolution_3d const &model)
      : m_cell(c), m_model(model)
  {
  }
  simbad::core::attribute get_attribute(std::size_t idx) const override
  {
    return m_model.new_attribute(m_cell, idx);
  }
};
}

std::size_t spacetime::size() const { return spacetime_super::size(); }
const core::attribute_descriptor &spacetime::descriptor() const
{
  static std::unique_ptr<simbad::core::attribute_descriptor> map_p;
  if(nullptr != map_p)
    return map_p;

  map_p.reset(new simbad::core::attribute_descriptor);
  using simbad::core::ATTRIBUTE_KIND;
  map_p->add_attribute(0, "position", ATTRIBUTE_KIND::POSITION, 3);
  map_p->add_attribute(1, "density", ATTRIBUTE_KIND::ACCUMULATED, 1);
  map_p->add_attribute(2, "event.time", ATTRIBUTE_KIND::INFO, 1);
  map_p->add_attribute(3, "event.kind", ATTRIBUTE_KIND::INFO, 1);
  map_p->add_attribute(4, "birth.efficiency", ATTRIBUTE_KIND::INTRINSIC, 1);
  map_p->add_attribute(5, "birth.resistance", ATTRIBUTE_KIND::INTRINSIC, 1);
  map_p->add_attribute(6, "lifespan.efficiency", ATTRIBUTE_KIND::INTRINSIC, 1);
  map_p->add_attribute(7, "lifespan.resistance", ATTRIBUTE_KIND::INTRINSIC, 1);
  map_p->add_attribute(8, "success.efficiency", ATTRIBUTE_KIND::INTRINSIC, 1);
  map_p->add_attribute(9, "success.resistance", ATTRIBUTE_KIND::INTRINSIC, 1);
  map_p->add_attribute(10, "birth.rate", ATTRIBUTE_KIND::OBSERVABLE, 1);
  map_p->add_attribute(11, "death.rate", ATTRIBUTE_KIND::OBSERVABLE, 1);
  map_p->add_attribute(12, "success.probability", ATTRIBUTE_KIND::OBSERVABLE,
                       1);
  map_p->add_attribute(13, "lifespan", ATTRIBUTE_KIND::OBSERVABLE, 1);
  map_p->add_attribute(14, "dummy", ATTRIBUTE_KIND::INTRINSIC, 1);
  return *map_p;
}

void spacetime::visit_records(record_visitor visitor) const {

    spacetime_super::visit([this, visitor](cell const &p) {
      particle_attr view(p, *this);
      visitor(view);
    });

}
END_NAMESPACE_PARAMETER_EVOLUTION_3D
