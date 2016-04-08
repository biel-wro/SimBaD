#include "particle_1d.hpp"

#include "event_1d.hpp"
#include "integrated_wave_1d.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

using namespace simbad::core;

particle_1d::particle_1d(float pos) : simple_particle<1, float>({pos}) {}

particle_1d::~particle_1d() {}

float particle_1d::get_position() const { return get_coordinate<0>(); }

void particle_1d::set_position(float pos) { set_coordinate<0>(pos); }

void particle_1d::update(const event_1d &e) { acc.update(*this, e); }

void particle_1d::update(const particle_1d &n) { acc.update(*this, n); }

const density_accumulator &particle_1d::get_accumulator() const { return acc; }

END_NAMESPACE_INTEGRATED_WAVE_1D
