#include "attribute_exceptions.hpp"
#include <string>
BEGIN_NAMESPACE_CORE

unrecognized_attribute_name::unrecognized_attribute_name(std::string name)
    : unrecognized_attribute(std::string("unrecognized attribute name `") +
                             name + "`"),
      m_attribute_name(std::move(name))
{
}

const std::string &unrecognized_attribute_name::get_name() const
{
  return m_attribute_name;
}
unrecognized_attribute_number::unrecognized_attribute_number(std::size_t attrno)
    : super(std::string("unrecognized attribute with id=`") +
            std::to_string(attrno) + "`"),
      m_attributeno(attrno)
{
}

std::size_t unrecognized_attribute_number::attribute_idx() const
{
  return m_attributeno;
}

END_NAMESPACE_CORE
