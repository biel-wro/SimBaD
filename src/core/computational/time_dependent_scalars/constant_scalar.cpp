#include "constant_scalar.hpp"

#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE

constant_scalar::constant_scalar(real_type val) : m_scalar(val) {}

constant_scalar::constant_scalar(property_tree const &pt) :
    m_scalar(pt.get<real_type>("scalar")){}

constant_scalar::real_type constant_scalar::operator()(real_type) const
{
  return 1.0;
}
END_NAMESPACE_CORE
