#include "interface/particle.hpp"
#include "interface/attribute.hpp"

#include "utils/attribute_exceptions.hpp"
#include <stdexcept>
BEGIN_NAMESPACE_CORE

particle::~particle() {}
std::size_t particle::id() const { return 0; }
attribute particle::get_attribute(std::size_t attrno) const
{
  return extra_attribute(attrno);
}

attribute particle::extra_attribute(std::size_t) const
{
  throw unrecognized_attribute("no extra attributes are defined");
}
END_NAMESPACE_CORE
