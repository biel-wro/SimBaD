#include "attribute_descriptor_record.hpp"
BEGIN_NAMESPACE_CORE
attribute_descriptor_record::attribute_descriptor_record(
    std::size_t attr_id, std::string name, ATTRIBUTE_KIND kind,
    std::size_t attribute_dimension)
    : m_attribute_id(attr_id),
      m_attribute_name(std::move(name)),
      m_kind(kind),
      m_attribue_dimension(attribute_dimension)
{
}
const std::string &attribute_descriptor_record::attribute_name() const
{
  return m_attribute_name;
}

void attribute_descriptor_record::set_attribute_name(
    const std::string &attribute_name)
{
  m_attribute_name = attribute_name;
}

std::size_t attribute_descriptor_record::attribute_idx() const
{
  return m_attribute_id;
}

void attribute_descriptor_record::set_attribute_id(std::size_t attribute_id)
{
  m_attribute_id = attribute_id;
}

ATTRIBUTE_KIND attribute_descriptor_record::kind() const
{
  return m_kind;
}

void attribute_descriptor_record::set_kind(
    ATTRIBUTE_KIND kind)
{
  m_kind = kind;
}

std::size_t attribute_descriptor_record::attribute_dimension() const
{
  return m_attribue_dimension;
}

void attribute_descriptor_record::set_attribute_dimension(
    std::size_t attribute_dimension)
{
  m_attribue_dimension = attribute_dimension;
}

END_NAMESPACE_CORE
