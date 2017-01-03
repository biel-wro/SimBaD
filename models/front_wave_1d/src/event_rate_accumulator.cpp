#include "event_rate_accumulator.hpp"

#include <algorithm>
#include <limits>
#include <random>

#include "death_rate_accumulator.hpp"
#include "density_accumulator.hpp"
#include "interface/event_kind.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D

using simbad::core::EVENT_KIND;

double event_rate_accumulator::s_interaction_range()
{
  return birth_rate_accumulator::s_range();
}

std::pair<float, simbad::core::EVENT_KIND>
event_rate_accumulator::sample_event(std::mt19937_64 &r)
{
  double birth_intensity = get_intensity();

  double death_time = std::numeric_limits<double>::infinity();
  double birth_time = std::exponential_distribution<>(birth_intensity)(r);

  std::pair<float, simbad::core::EVENT_KIND> retval;

  if (death_time < birth_time)
  {
    retval.first = float(death_time);
    retval.second = EVENT_KIND::REMOVED;
  }
  else
  {
    retval.first = float(birth_time);
    retval.second = EVENT_KIND::CREATED;
  }

  return retval;
}
END_NAMESPACE_FRONT_WAVE_1D
