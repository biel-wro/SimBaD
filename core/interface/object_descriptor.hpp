#ifndef OBJECT_DESCRIPTOR_HPP
#define OBJECT_DESCRIPTOR_HPP
#include "core_def.hpp"
#include <cstddef>
#include <string>

BEGIN_NAMESPACE_CORE
class object_descriptor
{
  object_descriptor(std::string name, std::size_t dim)
      : m_name(name), m_dimension(dim)
  {
  }

  std::string name() const { return m_name; }
  std::size_t dimension() const { return m_dimension; }

private:
  std::string m_name;
  std::size_t m_dimension;
};

END_NAMESPACE_CORE
#endif // OBJECT_DESCRIPTOR_HPP
