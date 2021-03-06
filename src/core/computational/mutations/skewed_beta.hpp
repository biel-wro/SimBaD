#ifndef SKEWED_BETA_MUTATION_HPP
#define SKEWED_BETA_MUTATION_HPP
#include "core_def.hpp"

#include "interface/mutator.hpp"
#include "interface/fwd/properties_fwd.hpp"

#include <random>
#include <stdexcept>

BEGIN_NAMESPACE_CORE
template <class RealType> class skewed_beta final : public mutator<RealType>
{
public:
  using real_type = RealType;
  skewed_beta(real_type alpha, real_type improvement_prob,
              real_type scale = real_type(1));
  
  explicit skewed_beta(property_tree const &pt);

  real_type find_beta(real_type parent_val) const;

  template <class RNG>
  real_type operator()(real_type parent_val, RNG &rng) const;

  real_type operator()(real_type parent_val,
                       std::mt19937_64 &rng) const override;

private:
  real_type m_alpha;
  real_type m_impr_prob;
  real_type m_scale;
};

END_NAMESPACE_CORE

#include "impl/skewed_beta_impl.hpp"

#endif
