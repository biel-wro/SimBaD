#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstddef>
#include <iosfwd>


#include "core_fwd.hpp"

BEGIN_NAMESPACE_CORE
class event
{
public:
  virtual double time() const = 0;
  virtual std::size_t dimension() const = 0;
  virtual std::size_t npartials() const = 0;

  virtual EVENT_KIND partial_type(std::size_t partialno) const = 0;
  virtual double coord(std::size_t partialno, std::size_t dimno) const = 0;

  virtual ~event();

  virtual void nice_print(std::ostream &) const;
  virtual void raw_text_print(std::ostream &) const;
};
END_NAMESPACE_CORE

std::ostream &operator<<(std::ostream &, CORE_NAMESPACE::event const &);
#endif // EVENT_HPP
