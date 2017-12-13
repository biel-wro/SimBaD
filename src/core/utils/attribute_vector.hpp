#ifndef ATTRIBUTE_VECTOR_HPP
#define ATTRIBUTE_VECTOR_HPP

#include "interface/interface_fwd.hpp"

#include "interface/attribute_list.hpp"

#include <vector>

BEGIN_NAMESPACE_CORE
class attribute_vector : public attribute_list
{
public:
  attribute_vector();
  attribute get_attribute(std::size_t idx) const override;
private:
  std::vector<attribute> m_attributes;
};
END_NAMESPACE_CORE
#endif // ATTRIBUTE_VECTOR_HPP
