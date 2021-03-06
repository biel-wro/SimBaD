#include "property_tree.hpp"

BEGIN_NAMESPACE_CORE
const property_tree &property_tree::get_empty()
{
  static property_tree pt;
  return pt;
}
property_tree::property_tree() {}
property_tree::property_tree(const property_tree::super &s) : super(s) {}
property_tree::property_tree(
    std::initializer_list<std::pair<std::string, std::string>> const &pairs)
{
  for(std::pair<std::string, std::string> const &pair : pairs)
  {
    put(pair.first, pair.second);
  }
}

std::size_t property_tree::total_size(bool ignore_empty) const
{
  std::size_t cnt = 0;
  visit([&cnt](std::string const &, std::string const &) { ++cnt; },
        ignore_empty);
  return cnt;
}

property_tree::~property_tree() {}
END_NAMESPACE_CORE
