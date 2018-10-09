#ifndef COMPUTATIONAL_FWD_HPP
#define COMPUTATIONAL_FWD_HPP

#include "core_def.hpp"

#include <functional>

BEGIN_NAMESPACE_CORE
// interface classes
class mutator;
class time_dependent_scalar;

// as std::function
using time_dependent_scalar_fun = std::function<double(double)>;

END_NAMESPACE_CORE

#endif // COMPUTATIONAL_FWD_HPP
