#ifndef SIMBAD_PROCESSORS_TREE_BUILDER_HPP
#define SIMBAD_PROCESSORS_TREE_BUILDER_HPP

#include "core_fwd.hpp"

#include <unordered_map>

BEGIN_NAMESPACE_CORE
class tree_builder
{
public:
  tree_builder(property_tree const &pt);

  void operator()(event_source &source, std::size_t read_size);

private:


};
END_NAMESPACE_CORE
#endif
