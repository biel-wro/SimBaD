#ifndef DEATH_RATE_ACCUMULATOR_HPP
#define DEATH_RATE_ACCUMULATOR_HPP

namespace simbad
{
namespace models
{

class death_rate_accumulator
{
public:
    death_rate_accumulator() : acc(0.0) {}

    template <class Point>
    void accumulate(Point const &p, Point const &other)
    {
        (void)p;
        (void)other;
    }

    double get_range() const { return 0.0; }
    double get_intensity() const { return 0.0; }

    double acc;
};
}
}
#endif // DEATH_RATE_ACCUMULATOR_HPP
