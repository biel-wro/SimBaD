#ifndef CORE_EVENT_SOURCE_HPP
#define CORE_EVENT_SOURCE_HPP
#include "core_fwd.hpp"
#include "dataframe.hpp"
#include <functional>

BEGIN_NAMESPACE_CORE
class event_source : public dataframe
{
public:
  using event_visitor = std::function<void(event const &)>;
  virtual void run(event_visitor, std::size_t) final;
  virtual void run(std::size_t) final;

  void visit_records( record_visitor visitor) const override;
  attribute_descriptor const &descriptor() const override;

  virtual ~event_source();

protected:
  virtual void generate_events(event_visitor, std::size_t) = 0;
};

END_NAMESPACE_CORE

#endif // EVENT_SOURCE_HPP
