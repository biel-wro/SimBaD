#ifndef ATTRIBUTE_EXCEPTIONS_HPP
#define ATTRIBUTE_EXCEPTIONS_HPP

#include "core_fwd.hpp"

#include <exception>
#include <stdexcept>
#include <string>
#include <typeinfo>
BEGIN_NAMESPACE_CORE

class unrecognized_attribute : public std::logic_error
{
public:
  using super = std::logic_error;
  using super::super;
};

class unrecognized_attribute_name : public unrecognized_attribute
{
public:
  unrecognized_attribute_name(std::string);
  std::string const &get_name() const;

private:
  std::string m_attribute_name;
};

class unrecognized_attribute_number : public unrecognized_attribute
{
public:
  using super = unrecognized_attribute;
  unrecognized_attribute_number(std::size_t attrno);
  std::size_t attribute_idx() const;

private:
  std::size_t m_attributeno;
};

class unrecognized_attribute_kind : public unrecognized_attribute
{
public: using super= unrecognized_attribute;
    unrecognized_attribute_kind(ATTRIBUTE_KIND kind);


};

class bad_attribute : public std::bad_cast
{
  using std::bad_cast::bad_cast;
};

END_NAMESPACE_CORE
#endif // ATTRIBUTE_EXCEPTIONS_HPP
