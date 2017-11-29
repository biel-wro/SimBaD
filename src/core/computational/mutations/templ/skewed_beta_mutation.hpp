#ifndef SKEWED_BETA_MUTATION_HPP
#define SKEWED_BETA_MUTATION_HPP
#include "core_def.hpp"

#include "interface/property_tree.hpp"

#include <boost/math/distributions/beta.hpp>

#include <random>
#include <stdexcept>
BEGIN_NAMESPACE_CORE
template <class RealType = double> class skewed_beta_mutator
{
public:
  using real_type = RealType;
  skewed_beta_mutator(real_type alpha, real_type improvement_prob,
                      real_type scale = real_type(1))
      : m_alpha(alpha), m_impr_prob(improvement_prob), m_scale(scale)
  {
  }
  skewed_beta_mutator(property_tree const &pt)
      : skewed_beta_mutator(pt.get<real_type>("alpha"),
                            pt.get<real_type>("improvement_probability"),
                            pt.get<real_type>("scale"))
  {
  }

  real_type find_beta(real_type parent_val) const
  {
    return boost::math::ibetac_invb(m_alpha, parent_val, m_impr_prob);
  }

  template <class RNG>
  real_type operator()(real_type parent_val, RNG &rng) const
  {
    real_type beta = find_beta(parent_val);
    real_type u = std::uniform_real_distribution<real_type>()(rng);
    boost::math::beta_distribution<real_type> beta_dist(m_alpha, beta);
    real_type x = boost::math::quantile(beta_dist, u);
    return x;
  }

private:
  real_type m_alpha;
  real_type m_impr_prob;
  real_type m_scale;
};

END_NAMESPACE_CORE

#endif // SKEWED_BETA_MUTATION_HPP
