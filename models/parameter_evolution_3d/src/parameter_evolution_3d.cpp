#include "parameter_evolution_3d.hpp"
#include <stdexcept>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

std::string parameter_evolution_3d::model_name()
{
  return "parameter_evolution_3d";
}
std::size_t parameter_evolution_3d::s_dimension() { return 3; }

std::size_t parameter_evolution_3d::dimension() const
{
  return s_dimension();
}
void parameter_evolution_3d::generate_events(event_visitor, size_type nevents)
{
  if(m_spacetime.empty())
    return;
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

void parameter_evolution_3d::resample_event(particle &p)
{
  particle::intrinsic_params const &intinisc_params = p.get_params();

}

void parameter_evolution_3d::generate_event(event_visitor v)
{
  switch(m_spacetime.top().get_event_kind())
  {
  case particle::EVENT_KIND::BIRTH: execute_birth(v); break;
  case particle::EVENT_KIND::DEATH: execute_birth(v); break;
  default: throw std::runtime_error("empty event in queue");
  }
}

void parameter_evolution_3d::execute_death(event_visitor v)
{
  particle const &p = m_spacetime.top();

  auto neighbor_visitor = [&p](particle &neighbor) {
    //neighbor.exclude_interaction(p);
    //neighbor.resample_event();
  };
  m_spacetime.visit_ball_guarded_order(p.get_coords(), m_interaction_range,
                                       neighbor_visitor);
  m_spacetime.pop();
}
void parameter_evolution_3d::execute_birth(event_visitor v) {}
END_NAMESPACE_PARAMETER_EVOLUTION_3D
