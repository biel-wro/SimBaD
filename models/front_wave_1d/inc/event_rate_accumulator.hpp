#ifndef EVENT_ACCUMULATOR_HPP
#define EVENT_ACCUMULATOR_HPP


#include "front_wave_1d_fwd.hpp"
#include <random>
#include "birth_rate_accumulator.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D

class event_rate_accumulator : public birth_rate_accumulator
{
public:
  static double s_interaction_range();

  std::pair<float, simbad::core::EVENT_KIND> sample_event(std::mt19937_64 &r);
};

END_NAMESPACE_FRONT_WAVE_1D
#endif // EVENT_ACCUMULATOR_HPP
