#ifndef SIMBAD_CORE_REPOSITORY_REGISTRATION_HPP
#define SIMBAD_CORE_REPOSITORY_REGISTRATION_HPP

#include "repository.hpp"

#include <string>

BEGIN_NAMESPACE_CORE
template <class Derived, class Interface>
void register_creator(repository<Interface> &repo, std::string name){
  repo.emplace(std::move(name),
               [](property_tree const &pt) {
                 return std::make_unique<Derived>(pt);
               });
}
END_NAMESPACE_CORE
#endif //SIMBAD_CORE_REPOSITORY_REGISTRATION_HPP
