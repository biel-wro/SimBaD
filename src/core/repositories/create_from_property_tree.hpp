#ifndef SIMBAD_CORE_CREATE_FROM_PROPERTY_TREE_HPP
#define SIMBAD_CORE_CREATE_FROM_PROPERTY_TREE_HPP

#include "interface/property_tree.hpp"
#include "repositories/factory.hpp"

#include <memory>

BEGIN_NAMESPACE_CORE
template <class Interface>
std::unique_ptr<Interface>
factory_create_from_property_tree(property_tree const &pt)
{
  std::string const &class_name = pt.get<std::string>("class");
  property_tree const &parameters_tree =
      pt.get_child("parameters", property_tree());

  std::unique_ptr<Interface> instance =
      factory<Interface>::global_instance().at(class_name)(parameters_tree);

  return instance;
}

END_NAMESPACE_CORE

#endif
