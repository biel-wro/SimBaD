#include "parameter_evolution_3d.hpp"
#include <stdexcept>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

void parameter_evolution_3d::generate_events(event_visitor, size_type nevents)
{
  throw std::runtime_error("not implemented yet");
}

parameter_evolution_3d::size_type
parameter_evolution_3d::configuration_size() const
{
  throw std::runtime_error("not implemented yet");
}

void parameter_evolution_3d::visit_configuration(particle_visitor) const
{
  throw std::runtime_error("not implemented yet");
}

END_NAMESPACE_PARAMETER_EVOLUTION_3D
