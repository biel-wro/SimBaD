#include "identity.hpp"

#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE

identity::identity() = default;
identity::identity(property_tree const &){}

identity::real_type identity::operator()(real_type, real_type val) const
{
  return val;
}
END_NAMESPACE_CORE
