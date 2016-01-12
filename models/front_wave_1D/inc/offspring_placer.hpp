#ifndef OFFSPRING_PLACER_HPP
#define OFFSPRING_PLACER_HPP
#include <boost/math/distributions/pareto.hpp>
#include <random>

namespace simbad
{
namespace models
{
class offspring_placer
{
  public:
    offspring_placer(double x0, double power) : dist(x0, power - 1) {}

    template <class RNG>
    double operator()(RNG &rng,double parent_pos) const
    {
        //int side = std::uniform_int_distribution<>(0, 1)(rng) * 2 - 1;

        double q = std::uniform_real_distribution<>()(rng);

        double v = boost::math::quantile(dist, q);
        return parent_pos + (v - dist.scale());
    }

  private:
    boost::math::pareto_distribution<> dist;
};
}
}

#endif // OFFSPRING_PLACER_HPP
