#include "interface/particle.hpp"

#include<stdexcept>
BEGIN_NAMESPACE_CORE

particle::~particle() {}
attribute
particle::extra_attribute(const std::string &attribute_name) const
{
  std::string msg = "attribute '" + attribute_name + "' not found";
  throw std::runtime_error(std::move(msg));
}
END_NAMESPACE_CORE
