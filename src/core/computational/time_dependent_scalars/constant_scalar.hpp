#ifndef CONSTANT_SCALAR_HPP
#define CONSTANT_SCALAR_HPP

#include "time_dependent_scalar.hpp"

#include "core_fwd.hpp"

BEGIN_NAMESPACE_CORE

class constant_scalar : public time_dependent_scalar
{
public:
  explicit constant_scalar(real_type val = 1.0);
  explicit constant_scalar(property_tree const &pt);
  real_type operator()(real_type) const override;

private:
  real_type m_scalar;
};

END_NAMESPACE_CORE

#endif // CONSTANT_SCALAR_HPP
