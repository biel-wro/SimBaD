#include "particle_1d.hpp"

#include "event_1d.hpp"


namespace simbad
{
namespace models
{

using namespace simbad::core;

particle_1D::particle_1D(float pos, simple_event_handle h)
    : simple_particle<1, float>({pos}, h)
{
}

void particle_1D::update_accumulators(const event_1d &e)
{
    acc.update(*this,e);
}

void particle_1D::update_accumulators(const particle_1D &p)
{
    acc.update(*this,p);
}

double particle_1D::interaction_range()
{
    return event_rate_accumulator::interaction_range();
}

std::pair<float, EVENT_KIND> particle_1D::sample_event(std::mt19937_64 &r)
{
    return acc.sample_event(r);
}



}
}
