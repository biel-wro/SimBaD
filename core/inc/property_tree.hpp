#ifndef CORE_PROPERTY_TREE_HPP
#define CORE_PROPERTY_TREE_HPP

#include "core_def.hpp"

#include "boost/property_tree/ptree.hpp"

BEGIN_NAMESPACE_CORE
class property_tree : public boost::property_tree::ptree
{
public:
  using super = boost::property_tree::ptree;
  using super::super;
  property_tree();
  property_tree(super const &s);
  ~property_tree();
};
END_NAMESPACE_CORE
#endif
