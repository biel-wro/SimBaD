#ifndef POWER_LAW_DISTRIBUTION_HPP
#define POWER_LAW_DISTRIBUTION_HPP

#include <boost/math/distributions.hpp>

namespace simbad
{
namespace core
{
template <class RealType, class Policy = boost::math::policies::policy<>>
class power_law_distribution
    : public boost::math::pareto_distribution<RealType, Policy>

{
public:
  using result_type = RealType;
  struct param_type
  {
    RealType sc, sh;
  };

  using my_super = boost::math::pareto_distribution<RealType, Policy>;
  explicit power_law_distribution(RealType scale = 1, RealType shape = 2)
      : my_super(scale, shape - RealType(1))
  {
  }
  explicit power_law_distribution(param_type const &p)
      : my_super(p.sc, p.sh - RealType(1))
  {
  }

  template <class URNG> result_type operator()(URNG &g)
  {
    using namespace boost::math;
    return quantile(complement(my_super(*this), g()));
  }
  template <class URNG> result_type operator()(URNG &g, param_type const &p)
  {
    return power_law_distribution(p)(g);
  }

  param_type param() const
  {
    return param_type{my_super::scale(), my_super::shape()};
  }
  void param(param_type const &p)
  {
    *this = power_law_distribution(p.sc, p.sh);
  }
  void reset() const {}
  RealType min() const { return my_super::scale(); }
  RealType max() const { return std::numeric_limits<RealType>::infinity(); }
};
}
}

namespace boost
{
namespace math
{
// 0arg
template <class R1, class P>
R1 variance(::simbad::core::power_law_distribution<R1, P> const &d)
{
  return variance(static_cast<pareto_distribution<R1, P> const &>(d));
}

template <class R1, class P>
R1 standard_deviation(::simbad::core::power_law_distribution<R1, P> const &d)
{
  return standard_deviation(static_cast<pareto_distribution<R1, P> const &>(d));
}

template <class R1, class P>
R1 median(::simbad::core::power_law_distribution<R1, P> const &d)
{
  return median(static_cast<pareto_distribution<R1, P> const &>(d));
}

template <class R1, class P>
R1 coefficient_of_variation(
    ::simbad::core::power_law_distribution<R1, P> const &d)
{
  return coefficient_of_variation(
      static_cast<pareto_distribution<R1, P> const &>(d));
}

// 1 arg
template <class R1, class P, class R2>
R1 pdf(::simbad::core::power_law_distribution<R1, P> const &d, R2 const &v)
{
  return pdf(static_cast<pareto_distribution<R1, P> const &>(d), v);
}

template <class R1, class P, class R2>
R1 cdf(::simbad::core::power_law_distribution<R1, P> const &d, R2 const &v)
{
  return cdf(static_cast<pareto_distribution<R1, P> const &>(d), v);
}

template <class R1, class P, class R2>
R1 quantile(::simbad::core::power_law_distribution<R1, P> const &d, R2 const &v)
{
  return quantile(static_cast<pareto_distribution<R1, P> const &>(d), v);
}

template <class R1, class P, class R2>
R1 hazard(::simbad::core::power_law_distribution<R1, P> const &d, R2 const &v)
{
  return hazard(static_cast<pareto_distribution<R1, P> const &>(d), v);
}

template <class R1, class P, class R2>
R1 chf(::simbad::core::power_law_distribution<R1, P> const &d, R2 const &v)
{
  return chf(static_cast<pareto_distribution<R1, P> const &>(d), v);
}

// 1 arg complement
template <class R1, class P, class R2>
R1 cdf(complemented2_type<::simbad::core::power_law_distribution<R1, P>,
                          R2> const &c)
{
  cdf(complement(static_cast<pareto_distribution<R1, P> const &>(c.dist),
                 c.param));
}

template <class R1, class P, class R2>
R1 quantile(complemented2_type<::simbad::core::power_law_distribution<R1, P>,
                               R2> const &c)
{
  quantile(complement(static_cast<pareto_distribution<R1, P> const &>(c.dist),
                      c.param));
}
}
}

#endif // POWER_LAW_DISTRIBUTION_HPP
