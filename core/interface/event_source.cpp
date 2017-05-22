#include "event_source.hpp"

#include "dataframe.hpp"

BEGIN_NAMESPACE_CORE
void event_source::run(event_source::event_visitor v, std::size_t sz)
{
  generate_events(v, sz);
}
void event_source::run(std::size_t sz)
{
  generate_events([](auto const &) {}, sz);
}

void event_source::visit_records(dataframe::record_visitor visitor) const
{
  throw "";
}

const attribute_descriptor &event_source::descriptor() const { throw ""; }
event_source::~event_source() {}
END_NAMESPACE_CORE
