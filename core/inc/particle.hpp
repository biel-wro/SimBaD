#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "core_fwd.hpp"

BEGIN_NAMESPACE_CORE
class particle
{
public:
  virtual ~particle();

  virtual std::size_t dimension() const = 0;
  virtual double coord(std::size_t d) const = 0;

};
END_NAMESPACE_CORE
#endif // PARTICLE_HPP
