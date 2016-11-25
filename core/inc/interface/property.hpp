#ifndef PROPERTY_HPP
#define PROPERTY_HPP
#include "core_def.hpp"

#include <boost/variant.hpp>

#include <string>

BEGIN_NAMESPACE_CORE
class attribute : public boost::variant<std::string, double, long int>
{
public:
  using super = boost::variant<std::string, double, long int>;
  using super::super;
};

END_NAMESPACE_CORE
#endif // PROPERTY_HPP
