#ifndef SIMPLE_EVENT_VIEW_HPP
#define SIMPLE_EVENT_VIEW_HPP

#include "core_fwd.hpp"
#include "interface/event.hpp"


BEGIN_NAMESPACE_CORE

template <class base_event>
class simple_event_view : public event
{

public:
  using base_ptr = const base_event *;

  simple_event_view() : pbase(nullptr) {}
  simple_event_view(base_event const &b) : pbase(&b) {}

  ~simple_event_view() {}

  bool has_assigned_base() { return nullptr != pbase; }

  double time() const override { return pbase->time(); }
  std::size_t dimension() const override { return base_event::s_dimension(); }
  std::size_t npartials() const override { return 1; }
  EVENT_KIND partial_type(std::size_t partialno) const override
  {
    (void)partialno; // remove warning, there is only one partial event
    return pbase->event_kind();
  }
  double coord(std::size_t partialno, std::size_t dimno) const override
  {
    (void)partialno; // remove warning, there is only one partial
    return pbase->coordinate(dimno);
  }

protected:
  base_ptr pbase;
};

END_NAMESPACE_CORE

#endif // SIMPLE_EVENT_VIEW_HPP
