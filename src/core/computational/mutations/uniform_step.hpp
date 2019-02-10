#ifndef SIMBAD_COMPUTATIONAL_MUTATORS_UNIFORM_STEP_MUTATOR_HPP
#define SIMBAD_COMPUTATIONAL_MUTATORS_UNIFORM_STEP_MUTATOR_HPP

#include "interface/interface_fwd.hpp"
#include "interface/mutator.hpp"

#include <random>
BEGIN_NAMESPACE_CORE
template <class RealType> class uniform_step final : public mutator<RealType>
{
public:
  using real_type = RealType;
  uniform_step(real_type increase_length, real_type decrease_length);
  explicit uniform_step(property_tree const &pt);

  template <class RNG>
  real_type operator()(real_type parent_val, RNG &rng) const;
  real_type operator()(real_type, std::mt19937_64 &rng) const override;

private:
  real_type m_increase_length;
  real_type m_decrease_length;
};

END_NAMESPACE_CORE

#include "impl/uniform_step_impl.hpp"

#endif
