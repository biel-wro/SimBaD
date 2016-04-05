#ifndef EVENT_ACCUMULATOR_HPP
#define EVENT_ACCUMULATOR_HPP
#include <random>

#include "birth_rate_accumulator.hpp"
#include "death_rate_accumulator.hpp"
#include "density_accumulator.hpp"

namespace simbad
{
namespace models
{
class event_rate_accumulator : public birth_rate_accumulator
{
  public:
    static double s_interaction_range();

    std::pair<float, simbad::core::EVENT_KIND> sample_event(std::mt19937_64 &r);
};
}
}
#endif // EVENT_ACCUMULATOR_HPP
