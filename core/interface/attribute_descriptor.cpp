#include "attribute_descriptor.hpp"
BEGIN_NAMESPACE_CORE
attribute_descriptor::attribute_descriptor(std::string name,
                                           std::size_t attr_id,
                                           ATTRIBUTE_KIND kind)
    : m_attribute_name(std::move(name)),
      m_attribute_id(attr_id),
      m_attribute_kind(kind)
{
}
const std::string &attribute_descriptor::attribute_name() const
{
  return m_attribute_name;
}

void attribute_descriptor::set_attribute_name(const std::string &attribute_name)
{
  m_attribute_name = attribute_name;
}

std::size_t attribute_descriptor::attribute_idx() const
{
  return m_attribute_id;
}

void attribute_descriptor::set_attribute_id(std::size_t attribute_id)
{
  m_attribute_id = attribute_id;
}

ATTRIBUTE_KIND attribute_descriptor::attribute_kind() const
{
  return m_attribute_kind;
}

void attribute_descriptor::set_attribute_kind(ATTRIBUTE_KIND attribute_kind)
{
  m_attribute_kind = attribute_kind;
}





END_NAMESPACE_CORE
