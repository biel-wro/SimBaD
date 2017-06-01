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



event_source::~event_source() {}
END_NAMESPACE_CORE
