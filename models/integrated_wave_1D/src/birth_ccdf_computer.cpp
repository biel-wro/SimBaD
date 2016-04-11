
#include "birth_ccdf_computer.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

birth_ccdf_computer::birth_ccdf_computer(double x) : x(x) {}
birth_ccdf_computer::~birth_ccdf_computer() {}

double birth_ccdf_computer::operator()(const particle_1d &p) const
{

    throw std::runtime_error("not implemented yet");
}

END_NAMESPACE_INTEGRATED_WAVE_1D
