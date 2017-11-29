#include "attribute_list.hpp"
#include "interface/attribute.hpp"
BEGIN_NAMESPACE_CORE
attribute_list::attribute_list() {}
attribute attribute_list::operator[](std::size_t idx) const
{
  attribute attr = get_attribute(idx);
  return attr;
}
attribute_list::~attribute_list() {}
END_NAMESPACE_CORE
