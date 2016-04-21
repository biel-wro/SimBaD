#ifndef BIRTH_RATE_ACCUMULATOR_HPP
#define BIRTH_RATE_ACCUMULATOR_HPP
#include <cmath>
#include <stdint.h>

#include "density_accumulator.hpp"
#include "event_1d.hpp"
#include "event_kind.hpp"

namespace simbad
{
namespace models
{
class birth_rate_accumulator : public density_accumulator
{
  public:
    static constexpr int32_t saturation = 10;

    double get_density() const{
        return static_cast<double>(get_value() );
    }
    double get_intensity() const
    {
        double density = get_density();
        double saturation = s_saturation_level();
        return std::min( 1.0, saturation/density);
    }
    int32_t saturation_level() const { return s_saturation_level(); }

    static int32_t s_saturation_level() { return saturation; }

    bool is_saturated() const { return get_value() >= saturation_level(); }
};
}
}
#endif // BIRTH_RATE_ACCUMULATOR_HPP
