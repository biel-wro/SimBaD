#ifndef UNIFORM_STEP_MUTATOR_HPP
#define UNIFORM_STEP_MUTATOR_HPP

#include "core_def.hpp"
#include "interface/property_tree.hpp"
#include <random>
BEGIN_NAMESPACE_CORE
template <class RealType> class uniform_step_mutator
{
public:
  using real_type = RealType;
  uniform_step_mutator(real_type increase_lenght, real_type decrease_length)
      : m_increase_length(increase_lenght), m_decrease_length(decrease_length)
  {
  }
  uniform_step_mutator(property_tree const &pt)
      : uniform_step_mutator(pt.get<real_type>("decrease_length"),
                             pt.get<real_type>("increase_length"))
  {
  }

  template <class RNG>
  real_type operator()(real_type parent_val, RNG &rng) const
  {
    real_type min_val = std::max(real_type(0), parent_val - m_decrease_length);
    real_type max_val = std::min(real_type(1), parent_val + m_increase_length);
    return std::uniform_real_distribution<real_type>(min_val, max_val)(rng);
  }

private:
  real_type m_increase_length;
  real_type m_decrease_length;
};
END_NAMESPACE_CORE
#endif // UNIFORM_STEP_MUTATOR_HPP
