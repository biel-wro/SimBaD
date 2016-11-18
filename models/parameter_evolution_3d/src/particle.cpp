#include "particle.hpp"
#include "model_parameters.hpp"

#include <random>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
particle::particle() {}
particle::~particle() {}
const particle::coords &particle::get_coords() const { return m_coords; }
float particle::get_time() const { return m_time; }
particle::EVENT_KIND particle::get_event_kind() const { return m_event_kind; }
void particle::include_interaction(const particle &p,
                                   const model_parameters &mp)
{
  throw "not implemented yet";
}

void particle::exclude_interaction(const particle &p,
                                   const model_parameters &mp)
{
  throw "not implemented yet";
}

particle::intrinsic_params &particle::get_params()
{
  return m_intrinsic_params;
}

const particle::intrinsic_params &particle::get_params() const
{
  return m_intrinsic_params;
}

END_NAMESPACE_PARAMETER_EVOLUTION_3D
