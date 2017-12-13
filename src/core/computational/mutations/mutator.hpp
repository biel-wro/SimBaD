#ifndef MUTATOR_HPP
#define MUTATOR_HPP

#include "interface/interface_fwd.hpp"
#include <random>

BEGIN_NAMESPACE_CORE

class mutator
{
public:
  using real_type = double;
  mutator(property_tree const &pt);
  virtual real_type operator()(real_type parent_val,
                               std::mt19937_64 &rng) const = 0;
  virtual ~mutator();
};

END_NAMESPACE_CORE
#endif // MUTATOR_HPP
