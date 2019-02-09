#ifndef SIMBAD_COMPLEMENTARY_GENERALIZED_EXPONENTIAL_HPP
#define SIMBAD_COMPLEMENTARY_GENERALIZED_EXPONENTIAL_HPP

#include "generalized_exponential.hpp"
BEGIN_NAMESPACE_CORE
template <class Scalar>
class complementary_generalized_exponential
    : public generalized_exponential<Scalar>
{
public:
  using base = generalized_exponential<Scalar>;
  using base::base;
  using scalar_type = typename base::scalar_type;

  scalar_type operator()(scalar_type val) const override
  {
    if(std::numeric_limits<scalar_type>::infinity() == base::m_gamma)
      return val <= base::m_sigma ? 0 : base::m_scale;

    scalar_type exponent = -std::pow(val / base::m_sigma, base::m_gamma) /
        base::m_gamma;
    return base::m_scale * -std::expm1(exponent);
  }
};
END_NAMESPACE_CORE
#endif // SIMBAD_COMPLEMENTARY_GENERALIZED_EXPONENTIAL_HPP
