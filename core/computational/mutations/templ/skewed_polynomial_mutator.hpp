#ifndef SKEWED_POLYNOMIAL_MUTATOR_HPP
#define SKEWED_POLYNOMIAL_MUTATOR_HPP

#include "interface/property_tree.hpp"
#include <random>
#include <stdexcept>

BEGIN_NAMESPACE_CORE
template <class RealType = double> class skewed_polynomial_mutator
{
public:
  using real_type = RealType;

  static constexpr real_type max_value() { return 1; }
  static constexpr real_type min_value() { return 0; }
  skewed_polynomial_mutator(real_type improvement_prob,
                            real_type improvement_power,
                            real_type deterioration_power)
      : m_impr_prob(improvement_prob),
        m_improvement_exponent(improvement_power),
        m_deterioraiton_exponent(deterioration_power)
  {
  }

  skewed_polynomial_mutator(property_tree const &pt)
      : skewed_polynomial_mutator(pt.get<real_type>("improvement_probability"),
                                  pt.get<real_type>("improvement_exponent"),
                                  pt.get<real_type>("deterioration_exponent"))
  {
  }

  template <class RNG>
  real_type operator()(real_type parent_val, RNG &rng) const
  {
    real_type u = std::uniform_real_distribution<real_type>()(rng);

    if(u < m_impr_prob)
      return improve(parent_val, rng);
    return hinder(parent_val, rng);
  }

protected:
  template <class RNG> real_type improve(real_type parent_val, RNG &rng) const
  {
    real_type r = polyrnd_inv(m_improvement_exponent, rng);
    real_type increase = r * (max_value() - parent_val);
    return std::min(parent_val + increase, max_value());
  }
  template <class RNG> real_type hinder(real_type parent_val, RNG &rng) const
  {
    real_type r = polyrnd_inv(m_deterioraiton_exponent, rng);
    real_type decrease = r * (parent_val - min_value());
    return std::max(parent_val - decrease, min_value());
  }
  /*
   * samples random number from distribution given by pdf:
   * x^alpha, where x = (0:1), alpha > 0
   */
  template <class RNG> real_type polyrnd(real_type power, RNG &rng) const
  {
    real_type q = std::uniform_real_distribution<real_type>()(rng);
    return std::pow(q, 1 / (power + 1));
  }

  /*
   * samples random number fromdistribution given by pdf
   * (1-x)^alpha
   */
  template <class RNG> real_type polyrnd_inv(real_type power, RNG &rng) const
  {
    return 1 - polyrnd(power, rng);
  }

private:
  real_type m_impr_prob;
  real_type m_improvement_exponent;
  real_type m_deterioraiton_exponent;
};

END_NAMESPACE_CORE

#endif // SKEWED_POLYNOMIAL_MUTATOR_HPP
