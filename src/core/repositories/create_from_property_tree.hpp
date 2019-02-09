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
      pt.get_child("parameters", property_tree::get_empty());

  factory<Interface> const &f = factory<Interface>::global_instance();

  auto it = f.find(class_name);
  if(f.end() == it)
    throw std::out_of_range(
        "could not find name `" + class_name + "` for interface `" +
        boost::typeindex::type_id<Interface>().pretty_name() + "`");

  std::unique_ptr<Interface> instance_ptr = it->second(parameters_tree);

  return instance_ptr;
}

template <class Interface>
std::unique_ptr<Interface>
factory_create_from_property_tree(property_tree const &pt,
                                  std::string const &child_path)
{
  return factory_create_from_property_tree<Interface>(
      pt.get_child(child_path, property_tree::get_empty()));
}

END_NAMESPACE_CORE

#endif
