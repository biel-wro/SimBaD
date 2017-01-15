#ifndef CORE_PARTICLE_HPP
#define CORE_PARTICLE_HPP

#include "core_fwd.hpp"

#include <cstddef>
#include <exception>
#include <string>

BEGIN_NAMESPACE_CORE
class particle
{
public:
  virtual ~particle();

  virtual double coord(std::size_t d) const = 0;
  virtual std::size_t id() const;

  virtual attribute get_attribute(std::size_t attributeno) const;
protected:
  virtual attribute extra_attribute(std::size_t attrno) const;
};
END_NAMESPACE_CORE
#endif
