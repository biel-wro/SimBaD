#include "event_source.hpp"

BEGIN_NAMESPACE_CORE
void event_source::run(event_source::event_visitor v, size_t sz) {
  generate_events(v,sz);
}
void event_source::run(size_t sz) {
  generate_events([](auto const &){}, sz);
}
event_source::~event_source() {}
END_NAMESPACE_CORE
