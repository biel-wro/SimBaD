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
  virtual std::size_t partials_left() const = 0;
  virtual EVENT_KIND event_kind() const = 0;
  virtual particle const &subject() const = 0;

  virtual ~event();

  virtual void nice_print(std::ostream &) const;

};
END_NAMESPACE_CORE

std::ostream &operator<<(std::ostream &, CORE_NAMESPACE::event const &);
#endif // EVENT_HPP
