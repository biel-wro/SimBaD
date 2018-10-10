#ifndef SIMBAD_CORE_FACTORY_HPP
#define SIMBAD_CORE_FACTORY_HPP

#include "interface/interface_fwd.hpp"
#include "repositories/repositories_fwd.hpp"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

BEGIN_NAMESPACE_CORE

template <class Interface>
using creator_signature = std::unique_ptr<Interface>(property_tree const &);

template <class Interface>
using creator_function = std::function<creator_signature<Interface>>;

template <class Interface>
using factory_base =
    std::unordered_map<std::string, creator_function<Interface>>;

template <class Interface> struct factory : public factory_base<Interface>
{
  using factory_base<Interface>::factory_base;

  static factory<Interface> &global_instance_mutable()
  {
    static factory<Interface> repository = make_global_repository<Interface>();
    return repository;
  }
  static factory<Interface> const &global_instance()
  {
    return global_instance_mutable();
  }
};

END_NAMESPACE_CORE

#endif // SIMBAD_CORE_FACTORY_HPP
