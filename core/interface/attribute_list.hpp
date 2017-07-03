#ifndef ATTRIBUTE_LIST_HPP
#define ATTRIBUTE_LIST_HPP

#include "interface/interface_fwd.hpp"

BEGIN_NAMESPACE_CORE
class attribute_list
{
public:
  attribute_list();
  attribute operator[](std::size_t idx) const;
  virtual ~attribute_list();

protected:
  virtual attribute get_attribute(std::size_t idx) const = 0;
};
END_NAMESPACE_CORE
#endif // ATTRIBUTE_LIST_HPP
