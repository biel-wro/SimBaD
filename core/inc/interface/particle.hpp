#ifndef CORE_PARTICLE_HPP
#define CORE_PARTICLE_HPP

#include "core_def.hpp"
#include "property.hpp"

#include <cstddef>
#include <exception>
#include <string>

BEGIN_NAMESPACE_CORE
class particle
{
public:
  virtual ~particle();

  virtual std::size_t dimension() const = 0;
  virtual double coord(std::size_t d) const = 0;
  virtual attribute extra_attribute(std::string const &msg) const;
};
END_NAMESPACE_CORE
#endif
