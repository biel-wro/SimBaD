#ifndef SIMBAD_COMPUTATIONAL_MUTATORS_SKEWED_POLYNOMIAL_MUTATOR_HPP
#define SIMBAD_COMPUTATIONAL_MUTATORS_SKEWED_POLYNOMIAL_MUTATOR_HPP

#include "interface/fwd/computational_fwd.hpp"
#include "interface/fwd/properties_fwd.hpp"
#include "interface/mutator.hpp"

#include <random>
#include <stdexcept>

BEGIN_NAMESPACE_CORE
template <class RealType>
class skewed_polynomial final : public mutator<RealType>
{
public:
  using real_type = RealType;

  static constexpr real_type max_value() { return 1; }
  static constexpr real_type min_value() { return 0; }
  skewed_polynomial(real_type improvement_prob, real_type improvement_power,
                    real_type deterioration_power);

  explicit skewed_polynomial(property_tree const &pt);

  template <class RNG> real_type operator()(real_type, RNG &rng) const;
  real_type operator()(real_type, std::mt19937_64 &rng) const override;

protected:
  template <class RNG> real_type improve(real_type parent_val, RNG &rng) const;
  template <class RNG> real_type hinder(real_type parent_val, RNG &rng) const;
  /*
   * samples random number from distribution given by pdf:
   * x^alpha, where x = (0:1), alpha > 0
   */
  template <class RNG> real_type polyrnd(real_type power, RNG &rng) const;

  /*
   * samples random number from distribution given by pdf
   * (1-x)^alpha
   */
  template <class RNG> real_type polyrnd_inv(real_type power, RNG &rng) const;

private:
  real_type m_impr_prob;
  real_type m_improvement_exponent;
  real_type m_deterioraiton_exponent;
};

END_NAMESPACE_CORE

#include "impl/skewed_polynomial_impl.hpp"

#endif

