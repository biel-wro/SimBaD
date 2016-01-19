#include "event_rate_accumulator.hpp"

#include <algorithm>
#include <random>

#include "event_kind.hpp"
namespace simbad
{
namespace models
{
using simbad::core::EVENT_KIND;

double event_rate_accumulator::interaction_range()
{
    return std::max(birth_rate_accumulator::interaction_range(),
                    death_rate_accumulator::interaction_range());
}

std::pair<float, simbad::core::EVENT_KIND>
event_rate_accumulator::sample_event(std::mt19937_64 &r)
{
    double death_intensity = death_acc.get_intensity();
    double birth_intensity = birth_acc.get_intensity();

    double death_time = std::exponential_distribution<>(death_intensity)(r);
    double birth_time = std::exponential_distribution<>(birth_intensity)(r);

    std::pair<float, simbad::core::EVENT_KIND> retval;

    if (death_time < birth_time)
    {
        retval.first = death_time;
        retval.second = EVENT_KIND::DEATH;
    }
    else
    {
        retval.first = birth_time;
        retval.second = EVENT_KIND::BIRTH;
    }

    return retval;
}
}
}
