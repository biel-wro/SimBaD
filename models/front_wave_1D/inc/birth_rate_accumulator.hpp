#ifndef BIRTH_RATE_ACCUMULATOR_HPP
#define BIRTH_RATE_ACCUMULATOR_HPP

#include "event_1d.hpp"
#include "event_kind.hpp"

#include <cmath>
#include <stdint.h>
namespace simbad
{
namespace models
{
class birth_rate_accumulator
{
  public:
    birth_rate_accumulator() : cnt(0) {}

    void reset() { cnt = 0; }
    template <class Point, class Event>
    void update(Point const &p, Event const &e)
    {
        using core::EVENT_KIND;

        EVENT_KIND ek = e.event_kind();
        double point_pos = p.coordinate(0);
        double event_pos = e.coordinate(0);

        double distance = std::fabs(point_pos - event_pos);
        switch (ek)
        {
        case EVENT_KIND::BIRTH:
            update_on_creation(distance);
            break;
        case EVENT_KIND::DEATH:
            update_on_anihillation(distance);
            break;
        default:
            throw std::runtime_error("event not supported");
        }
    }
    template <class Point>
    void update(Point const &p1, Point const &p2)
    {
        double pos1 = p1.coordinate(0);
        double pos2 = p2.coordinate(0);
        double distance = std::fabs(pos1 - pos2);
        update_on_creation(distance);
    }

    void update_on_creation(double dist)
    {
        if (dist < interaction_range())
            cnt++;
    }
    void update_on_anihillation(double dist)
    {
        if (dist < interaction_range())
            cnt--;
    }

    double get_intensity() const { return cnt < 100 ? 1 : 0; }
    static double interaction_range() { return 1.0; }

    int32_t cnt;
};
}
}
#endif // BIRTH_RATE_ACCUMULATOR_HPP
