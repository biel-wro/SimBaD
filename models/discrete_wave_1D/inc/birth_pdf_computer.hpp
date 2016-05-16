#ifndef BIRTH_PDF_COMPUTER_HPP
#define BIRTH_PDF_COMPUTER_HPP

#include "integrated_wave_1d.hpp"
#include "particle_1d.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

class birth_pdf_computer
{
public:
    birth_pdf_computer(double x);
    ~birth_pdf_computer();
    double operator()(particle_1d const &p) const;
private:
    double x;
};

END_NAMESPACE_INTEGRATED_WAVE_1D

#endif
