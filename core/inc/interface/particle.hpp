#ifndef CORE_PARTICLE_HPP
#define CORE_PARTICLE_HPP

#include "core_def.hpp"
#include <cstddef>

BEGIN_NAMESPACE_CORE
class particle
{
public:
  virtual ~particle();

  virtual std::size_t dimension() const = 0;
  virtual double coord(std::size_t d) const = 0;

};
END_NAMESPACE_CORE
#endif
