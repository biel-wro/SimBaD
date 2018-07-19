#ifndef SIMBAD_CORE_REPOSITORIES_HPP
#define SIMBAD_CORE_REPOSITORIES_HPP

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
using repository_base =
    std::unordered_map<std::string, creator_function<Interface>>;

template <class Interface> struct repository : public repository_base<Interface>
{
  using repository_base<Interface>::repository_base;

  static repository<Interface> &global_instance_mutable()
  {
    static repository<Interface> repository =
        make_global_repository<Interface>();
    return repository;
  }
  static repository<Interface> const &global_instance()
  {
    return global_instance_mutable();
  }
};

END_NAMESPACE_CORE

#endif // SIMBAD_CORE_REPOSITORIES_HPP
