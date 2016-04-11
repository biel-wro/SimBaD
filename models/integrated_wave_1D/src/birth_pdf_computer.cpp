#include "birth_pdf_computer.hpp"

#include "cmath"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

birth_pdf_computer::birth_pdf_computer(double x) : x(x) {}

birth_pdf_computer::~birth_pdf_computer() {}

double birth_pdf_computer::operator()(const particle_1d &p) const
{
    double density = p.get_accumulator().get_value();
    double ppos = p.get_position();

    double dist = std::abs(ppos - x);

    throw std::runtime_error("not implemented yet");
}

END_NAMESPACE_INTEGRATED_WAVE_1D
