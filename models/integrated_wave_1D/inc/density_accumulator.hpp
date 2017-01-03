#ifndef DENSITY_ACCUMULATOR_HPP
#define DENSITY_ACCUMULATOR_HPP
#include <cmath>
#include <stdint.h>


#include "event_kind.hpp"
#include "integrated_wave_1d.hpp"


BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

class density_accumulator
{
  public:
    static constexpr double s_range_val = 1;
    density_accumulator() : cnt(0) {}
    void reset() { cnt = 0; }

    template <class Point, class Event>
    void update(Point const &p, Event const &e)
    {
        using simbad::core::EVENT_KIND;

        EVENT_KIND ek = e.event_kind();
        double point_pos = p.coordinate(0);
        double event_pos = e.coordinate(0);

        double distance = std::fabs(point_pos - event_pos);
        switch (ek)
        {
        case EVENT_KIND::BIRTH:
            update_on_creation(distance);
            break;
        case EVENT_KIND::REMOVED:
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
        if (dist < s_range())
            cnt++;
    }
    void update_on_anihillation(double dist)
    {
        if (dist < s_range())
            cnt--;
    }

    int32_t get_value() const { return cnt; }

    double range(){ return s_range(); }
    static double s_range() { return s_range_val; }
  private:
    int32_t cnt;
};

END_NAMESPACE_INTEGRATED_WAVE_1D
#endif // DENSITY_ACCUMULATOR_HPP
