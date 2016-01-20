#ifndef EVENT_ACCUMULATOR_HPP
#define EVENT_ACCUMULATOR_HPP
#include <random>

#include "birth_rate_accumulator.hpp"
#include "death_rate_accumulator.hpp"

namespace simbad
{
namespace models
{
class event_rate_accumulator
{

  public:
    void reset()
    {
        birth_acc.reset();
        death_acc.reset();
    }

    template <class Point, class Event>
    void update(Point const &p, Event const &e)
    {
        birth_acc.update(p, e);
        death_acc.update(p, e);
    }

    template <class Point>
    void update(Point const &p1, Point const &p2)
    {
        birth_acc.update(p1, p2);
        death_acc.update(p1, p2);
    }

    static double interaction_range();

    std::pair<float, simbad::core::EVENT_KIND> sample_event(std::mt19937_64 &r);

  private:
    birth_rate_accumulator birth_acc;
    death_rate_accumulator death_acc;
};
}
}
#endif // EVENT_ACCUMULATOR_HPP
