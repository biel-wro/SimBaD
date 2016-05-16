#ifndef INTENSITY_ACCUMULATOR_HPP
#define INTENSITY_ACCUMULATOR_HPP

#include <cmath>
#include <stdint.h>

#include "event_1d.hpp"
#include "event_kind.hpp"

#include "fixed/fixed.hpp"

#include "integrated_wave_1d.hpp"
#include "integrated_wave_1d_fwd.hpp"
#include "intensity_computer.hpp"
BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

class intensity_accumulator
{

  public:
    using accumulated_type = simbad::core::fixed::Fixed<64, 32>;
    enum class DIRECTION : bool
    {
        EXCLUDE,
        INCLUDE
    };

    static constexpr double s_range_val = 1;

    intensity_accumulator() : cnt(0) {}
    void reset() { cnt = 0; }

    template <DIRECTION dir, class Point, class Computer>
    void accumulate(Point const &p, Computer computer = Computer())
    {
//        double density = p.get_accumulator().get_value();
        double intensity = computer(p);
        if (dir == DIRECTION::INCLUDE)
            cnt += intensity;
        else if (dir == DIRECTION::EXCLUDE)
            cnt -= intensity;
    }

    double get_value() const { return cnt.to<double>(); }

    double range() { return s_range(); }
    static double s_range() { return s_range_val; }
  private:
    accumulated_type cnt;
};

END_NAMESPACE_INTEGRATED_WAVE_1D
#endif // DENSITY_ACCUMULATOR_HPP
