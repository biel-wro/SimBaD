#include "particle.hpp"
#include "model_parameters.hpp"

#include <random>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
cell::cell() {}
cell::~cell() {}
const cell::coords &cell::get_coords() const { return m_coords; }
float cell::get_time() const { return m_time; }
cell::EVENT_KIND cell::get_event_kind() const { return m_event_kind; }
void cell::include_interaction(const cell &p, const model_params &mp)
{
  throw "not implemented yet";
}

void cell::exclude_interaction(const cell &p, const model_params &mp)
{
  throw "not implemented yet";
}

cell_params &cell::get_params() { return m_intrinsic_params; }
const cell_params &cell::get_params() const { return m_intrinsic_params; }

void cell::mutate(const model_params &mp, std::mt19937_64 &rng)
{
  mp.mutate(m_intrinsic_params,rng);
}

END_NAMESPACE_PARAMETER_EVOLUTION_3D
