#include "property_tree.hpp"
BEGIN_NAMESPACE_CORE
property_tree::property_tree() {}
property_tree::property_tree(const property_tree::super &s) : super(s) {}
std::size_t property_tree::total_size(bool ignore_empty) const
{
  std::size_t cnt = 0;
  visit([&cnt](std::string const &, std::string const &) { ++cnt; },
        ignore_empty);
  return cnt;
}


property_tree::~property_tree() {}
END_NAMESPACE_CORE
