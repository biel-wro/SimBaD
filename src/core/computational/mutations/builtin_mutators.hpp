#ifndef BUILTIN_MUTATORS_HPP
#define BUILTIN_MUTATORS_HPP
#include "computational/computational_fwd.hpp"
#include "interface/interface_fwd.hpp"

#include "mutator.hpp"

BEGIN_NAMESPACE_CORE
class_register<mutator> const &get_builtin_mutators();
END_NAMESPACE_CORE
#endif // BUILTIN_MUTATORS_HPP
