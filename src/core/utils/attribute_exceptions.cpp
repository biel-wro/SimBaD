#include "attribute_exceptions.hpp"

#include "interface/attribute_kind.hpp"

#include <sstream>
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

static std::underlying_type_t<ATTRIBUTE_KIND>
kind_as_integer(ATTRIBUTE_KIND kind)
{
  return static_cast<std::underlying_type_t<ATTRIBUTE_KIND>>(kind);
}

unrecognized_attribute_kind::unrecognized_attribute_kind(ATTRIBUTE_KIND kind)
    : base("unrecognized attribute kind no`" +
           std::to_string(kind_as_integer(kind)) + "`")
{
}

unrecognized_special_attribute_kind::unrecognized_special_attribute_kind(
    ATTRIBUTE_KIND kind)
    : base("unrecognized special attribute kind `" +
           std::to_string(kind_as_integer(kind)) + "`")
{
}
END_NAMESPACE_CORE
