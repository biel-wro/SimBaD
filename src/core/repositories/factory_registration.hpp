#ifndef SIMBAD_CORE_REPOSITORY_REGISTRATION_HPP
#define SIMBAD_CORE_REPOSITORY_REGISTRATION_HPP

#include "factory.hpp"

#include <memory>
#include <string>

#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE
template <class Derived, class Interface>
void register_creator(factory<Interface> &repo, std::string name)
{
  std::unique_ptr<Derived> (*creator_ptr)(property_tree const &) =
      std::make_unique<Derived, property_tree const &>;
  repo.emplace(std::move(name), std::move(creator_ptr));
}

END_NAMESPACE_CORE
#endif // SIMBAD_CORE_REPOSITORY_REGISTRATION_HPP
