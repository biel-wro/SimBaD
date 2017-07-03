#include "particle.hpp"
#include "model_parameters.hpp"

#include "interface/attribute.hpp"
#include "interface/configuration_view.hpp"
#include "interface/event_kind.hpp"
#include <cstddef>
#include <random>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
cell::cell(position_type pos, cell_params params)
    : m_position(pos),
      m_time(0),
      m_params(std::move(params)),
      m_interaction_acc(0),
      m_event_kind(simbad::core::EVENT_KIND::NONE)
{
}
cell::~cell() {}
cell::position_type const &cell::position() const { return m_position; }
void cell::set_position(const position_type &pos) { m_position = pos; }
void cell::set_event_time(float t) { m_time = t; }
float cell::event_time() const { return m_time; }
cell::EVENT_KIND cell::event_kind() const { return m_event_kind; }
void cell::set_event_kind(cell::EVENT_KIND ek) { m_event_kind = ek; }

void cell::reset_interaction() { m_interaction_acc = 0; }
void cell::include_interaction(const cell &p, const model_params &mp)
{
  position_type displacement = p.position() - position();
  position_scalar distance = displacement.hypot();
  m_interaction_acc += mp.interaction()(distance);
}

void cell::exclude_interaction(const cell &p, const model_params &mp)
{
  position_type displacement = p.position() - position();
  position_scalar distance = displacement.hypot();
  m_interaction_acc -= mp.interaction()(distance);
}

cell_params &cell::params() { return m_params; }
const cell_params &cell::params() const { return m_params; }
double cell::density() const { return m_interaction_acc; }
void cell::mutate(const model_params &mp, std::mt19937_64 &rng)
{
  mp.mutate(m_params, rng);
}

END_NAMESPACE_PARAMETER_EVOLUTION_3D
