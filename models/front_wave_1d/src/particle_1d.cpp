#include "particle_1d.hpp"

#include "birth_rate_accumulator.hpp"
#include "death_rate_accumulator.hpp"
#include "event_1d.hpp"
#include "event_rate_accumulator.hpp"
#include "legacy/simple/simple_event_queue.hpp"
#include "legacy/simple/simple_tracked_particle.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D

using namespace simbad::core;

particle_1D::particle_1D(float pos, simple_event_handle h)
    : simple_tracked_particle<1, float>({pos}, h)
{
}

const event_rate_accumulator &particle_1D::get_event_acc() const { return acc; }

void particle_1D::update_accumulators(const event_1d &e)
{
  acc.update(*this, e);
}

void particle_1D::update_accumulators(const particle_1D &p)
{
  acc.update(*this, p);
}

double particle_1D::interaction_range()
{
  return event_rate_accumulator::s_interaction_range();
}

std::pair<float, EVENT_KIND> particle_1D::sample_event(std::mt19937_64 &r)
{
  return acc.sample_event(r);
}

END_NAMESPACE_FRONT_WAVE_1D
