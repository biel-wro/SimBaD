#ifndef CORE_EVENT_SOURCE_HPP
#define CORE_EVENT_SOURCE_HPP
#include "interface/interface_fwd.hpp"
#include <functional>

BEGIN_NAMESPACE_CORE
class event_source
{
public:
  using event_visitor =
      std::function<void(const simbad::core::attribute_list &)>;

  virtual void run(event_visitor, std::size_t) final;
  virtual void run(std::size_t) final;

  virtual attribute_description const &event_descriptor() const = 0;

  virtual ~event_source();

protected:
  virtual void generate_events(event_visitor, std::size_t) = 0;
};

END_NAMESPACE_CORE

#endif // EVENT_SOURCE_HPP
