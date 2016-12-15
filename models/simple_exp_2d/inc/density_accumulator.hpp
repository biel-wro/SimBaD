#ifndef DENSITY_ACCUMULATOR_HPP
#define DENSITY_ACCUMULATOR_HPP
#include "simple_exp_2d_def.hpp"

#include "computational/templates/accumulator.hpp"
#include "kernel.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D
class density_accumulator : public simbad::core::additive_accumulator<float>
{
public:
  using base_type = simbad::core::additive_accumulator<float>;
  void include(kernel k,particle const &p1, particle const &p2)
  {
    base_type::include(k,p1,p2);
  }
};


END_NAMESPACE_SIMPLE_EXP_2D

#endif
