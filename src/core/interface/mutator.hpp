#ifndef MUTATOR_HPP
#define MUTATOR_HPP

#include "core_def.hpp"

#include <random>

BEGIN_NAMESPACE_CORE

template<class Scalar>
class mutator
{
public:
  using real_type = Scalar;
  virtual real_type operator()(real_type parent_val,
                               std::mt19937_64 &rng) const = 0;
  virtual ~mutator();
};

END_NAMESPACE_CORE
#endif // MUTATOR_HPP
