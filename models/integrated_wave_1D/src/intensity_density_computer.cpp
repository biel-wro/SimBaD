#include "intensity_density_computer.hpp"

#include "cmath"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

intensity_density_computer::intensity_density_computer(double x) : x(x) {}

intensity_density_computer::~intensity_density_computer() {}

double intensity_density_computer::operator()(const particle_1d &p) const
{
    double density = p.get_accumulator().get_value();
    double ppos = p.get_position();

    double dist = std::abs(ppos - x);

    throw std::runtime_error("not implemented yet");
}

END_NAMESPACE_INTEGRATED_WAVE_1D
