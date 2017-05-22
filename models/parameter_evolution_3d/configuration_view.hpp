#ifndef PARAMETER_EVOLUTION_3D_CONFIGURATION_VIEW
#define PARAMETER_EVOLUTION_3D_CONFIGURATION_VIEW

#include "parameter_evolution_3d_fwd.hpp"

#include "core_fwd.hpp"

#include <cstddef>

#include "interface/configuration_view.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
class configuration_view : public simbad::core::configuration_view
{
public:
  configuration_view(parameter_evolution_3d const &model) : m_model(model) {}
  std::size_t size() const override;
  void visit_records(record_visitor visitor) const override;
  simbad::core::attribute_descriptor const &descriptor() const override ;
private:
  parameter_evolution_3d const &m_model;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif
