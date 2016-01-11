#ifndef BIRTH_RATE_ACCUMULATOR_HPP
#define BIRTH_RATE_ACCUMULATOR_HPP

#include <stdint.h>

namespace simbad
{
namespace models
{
class birth_rate_accumulator
{
public:
    birth_rate_accumulator() : range(1.0), cnt(100) {}

    template <class Point>
    void accumulate(Point const &p, Point const &other)
    {
        double x0 = static_cast<double>(p.template get_coordinate<0>());
        double x1 = static_cast<double>(other.template get_coordinate<0>());
        double dx = std::fabs(x0 - x1);
        //    double dy = p.get_coordinate<1>() - other.get_coordinate<1>();
        if (dx < get_range())
            cnt++;
    }

    double get_intensity() const { return cnt < 0 ? 0 : 1; }
    double get_range() const { return range; }

    double range;
    int32_t cnt;
};
}
}
#endif // BIRTH_RATE_ACCUMULATOR_HPP
