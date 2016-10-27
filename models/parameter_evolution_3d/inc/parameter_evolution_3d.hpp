#include "parameter_evolution_3d_def.hpp"
#include "model.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

class parameter_evolution_3d : public simbad::core::model
{
public:
  void generate_events(event_visitor, size_type nevents) override;
  size_type configuration_size() const override;
  void visit_configuration(particle_visitor) const override;

private:
};

END_NAMESPACE_PARAMETER_EVOLUTION_3D
