#ifndef ATTRIBUTE_MAPPING_HPP
#define ATTRIBUTE_MAPPING_HPP
#include "core_fwd.hpp"

#include <string>
#include <unordered_map>
#include <vector>

BEGIN_NAMESPACE_CORE

class attribute_mapping
{
public:
  static std::unordered_map<std::size_t, std::size_t>
  remap(attribute_description const &from, attribute_description const &to);
};
END_NAMESPACE_CORE
#endif // ATTRIBUTE_MAPPING_HPP
