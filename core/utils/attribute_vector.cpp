#include "attribute_vector.hpp"

#include "interface/attribute.hpp"
#include "utils/attribute_exceptions.hpp"

BEGIN_NAMESPACE_CORE
attribute_vector::attribute_vector() {}

attribute attribute_vector::get_attribute(std::size_t idx) const
{
  if(m_attributes.size() >= idx)
    throw unrecognized_attribute_number(idx);
  return m_attributes[idx];
}
END_NAMESPACE_CORE
