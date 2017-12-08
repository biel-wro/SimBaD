#include "attribute_descriptor.hpp"

BEGIN_NAMESPACE_CORE

attribute_descriptor::attribute_descriptor(std::size_t attr_id,
                                           std::string name,
                                           ATTRIBUTE_KIND kind,
                                           ATTRIBUTE_SCALAR scalar,
                                           size_t attribute_dimension)
    : m_attribute_id(attr_id),
      m_attribute_name(std::move(name)),
      m_kind(kind),
      m_scalar(scalar),
      m_attribue_dimension(attribute_dimension)
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

ATTRIBUTE_KIND attribute_descriptor::kind() const { return m_kind; }
void attribute_descriptor::set_kind(ATTRIBUTE_KIND kind) { m_kind = kind; }
std::size_t attribute_descriptor::attribute_dimension() const
{
  return m_attribue_dimension;
}

void attribute_descriptor::set_attribute_dimension(
    std::size_t attribute_dimension)
{
  m_attribue_dimension = attribute_dimension;
}

ATTRIBUTE_SCALAR attribute_descriptor::scalar() const { return m_scalar; }
void attribute_descriptor::set_scalar(ATTRIBUTE_SCALAR scalar)
{
  m_scalar = scalar;
}

std::ostream &operator<<(std::ostream &os, attribute_descriptor const &desc)
{
  os << "id=" << desc.attribute_idx()         //
     << " name=" << desc.attribute_name()     //
     << " dim=" << desc.attribute_dimension() //
     << " scalar=" << desc.scalar()           //
     << " kind=" << desc.kind();              //

  return os;
}



END_NAMESPACE_CORE
