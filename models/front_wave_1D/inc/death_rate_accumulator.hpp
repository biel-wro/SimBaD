#ifndef DEATH_RATE_ACCUMULATOR_HPP
#define DEATH_RATE_ACCUMULATOR_HPP

namespace simbad
{
namespace models
{

class death_rate_accumulator
{
  public:
    death_rate_accumulator() {}

    void reset() {}
    template <class Point>
    void accumulate(Point const &p, Point const &other)
    {
        (void)p;
        (void)other;
    }

    template <class Point, class Event>
    void update(Point const &, Event const &)
    {
        (void)0;
    }
    template<class Point>
    void update(Point const&, Point const&)
    {
        (void)0;
    }


    static double interaction_range() { return 0.0; }
    double get_intensity() const { return 0.0; }
};
}
}
#endif // DEATH_RATE_ACCUMULATOR_HPP
