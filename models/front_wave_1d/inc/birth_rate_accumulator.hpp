#ifndef BIRTH_RATE_ACCUMULATOR_HPP
#define BIRTH_RATE_ACCUMULATOR_HPP

#include "front_wave_1d_fwd.hpp"

#include "density_accumulator.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D

class birth_rate_accumulator : public density_accumulator
{
public:
  static constexpr int32_t saturation = 10;

  double get_density() const;
  double get_intensity() const;
  int32_t saturation_level() const;

  static int32_t s_saturation_level();

  bool is_saturated() const;
};

END_NAMESPACE_FRONT_WAVE_1D

#endif // BIRTH_RATE_ACCUMULATOR_HPP
