#include "birth_rate_accumulator.hpp"

#include <cmath>

#include "event_1d.hpp"
#include "event_kind.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D

double birth_rate_accumulator::get_density() const
{
  return static_cast<double>(get_value());
}

double birth_rate_accumulator::get_intensity() const
{
  double density = get_density();
  return std::min(1.0, s_saturation_level() / density);
}

int32_t birth_rate_accumulator::saturation_level() const
{
  return s_saturation_level();
}

int32_t birth_rate_accumulator::s_saturation_level() { return saturation; }

bool birth_rate_accumulator::is_saturated() const
{
  return get_value() >= saturation_level();
}

END_NAMESPACE_FRONT_WAVE_1D
