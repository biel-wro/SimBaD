#include "interface/event.hpp"

#include "event_kind.hpp"

#include <assert.h>
#include <ostream>
#include <type_traits>

BEGIN_NAMESPACE_CORE
/*
  event::~event() {}


void event::nice_print(std::ostream &o) const
{
  o << "t=" << this->time() << " -" << partials_left() << "d :";
  o << event_kind() << ":";

  std::size_t npartials = this->npartials();
  std::size_t ndims = this->dimension();

  for (std::size_t partialno = 0; partialno < npartials; ++partialno)
    nice_print_partial_event(o, *this, partialno, ndims);
}

void event::raw_text_print(std::ostream &o) const
{
  using EVENT_KIND_num = std::underlying_type<EVENT_KIND>::type;

  std::size_t npartials = this->npartials();
  std::size_t ndims = this->dimension();

  o << this->time();
  for(std::size_t partialno = 0; partialno < npartials; ++partialno)
  {
    o << " " << static_cast<EVENT_KIND_num>(this->partial_type(partialno));
    for(std::size_t dimno = 0; dimno < ndims; ++dimno)
      o << " " << this->coord(partialno, dimno) << " ";
  }

std::size_t event::dimension() const { return 0; }
std::size_t event::npartials() const { return 0; }
EVENT_KIND event::partial_type(std::size_t partialno) const
{
  return EVENT_KIND::NONE;
}

double event::coord(std::size_t partialno, std::size_t dimno) const
{
  return 0;
}

std::size_t event::partials_left() const { return 0; }
EVENT_KIND event::event_kind() const { return EVENT_KIND::NONE; }
const particle &event::subject() const
{
  assert(false);
  throw "err";
}
*/
END_NAMESPACE_CORE

std::ostream &operator<<(std::ostream &o, CORE_NAMESPACE::event const &e)
{
  //TODO
  //e.nice_print(o);
  return o;
}
