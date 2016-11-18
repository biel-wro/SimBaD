#include "particle.hpp"
#include "model_parameters.hpp"

#include <random>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
cell::cell() {}
cell::~cell() {}
const cell::coords &cell::get_coords() const { return m_coords; }
float cell::get_time() const { return m_time; }
cell::EVENT_KIND cell::get_event_kind() const { return m_event_kind; }
void cell::include_interaction(const cell &p,
                                   const model_parameters &mp)
{
  throw "not implemented yet";
}

void cell::exclude_interaction(const cell &p,
                                   const model_parameters &mp)
{
  throw "not implemented yet";
}

cell::intrinsic_params &cell::get_params()
{
  return m_intrinsic_params;
}

const cell::intrinsic_params &cell::get_params() const
{
  return m_intrinsic_params;
}

END_NAMESPACE_PARAMETER_EVOLUTION_3D
