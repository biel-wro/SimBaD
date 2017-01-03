#include "event_sink.hpp"

BEGIN_NAMESPACE_CORE
event_sink::~event_sink() {}
void event_sink::read_event(const event &e) { process_event(e); }
END_NAMESPACE_CORE
