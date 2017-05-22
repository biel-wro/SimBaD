#include "configuration_view.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "utils/attribute_converter.hpp"
#include "utils/attribute_exceptions.hpp"

#include "parameter_evolution_3d.hpp"
#include "particle_view.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

std::size_t configuration_view::size() const
{
  return m_model.current_spacetime().size();
}

const simbad::core::attribute_descriptor &configuration_view::descriptor() const
{
  static std::unique_ptr<simbad::core::attribute_descriptor> map_p;
  if(nullptr != map_p)
    return *map_p;

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

void configuration_view::visit_records(
    finite_dataframe::record_visitor visitor) const
{
  m_model.current_spacetime().visit([this, visitor](cell const &p) {
    particle_view view(p, m_model);
    visitor(view);
  });
}
END_NAMESPACE_PARAMETER_EVOLUTION_3D
