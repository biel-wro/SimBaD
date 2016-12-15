#include "interface/event.hpp"

#include "event_kind.hpp"
#include <ostream>
#include <type_traits>

BEGIN_NAMESPACE_CORE
event::~event() {}

static void nice_print_partial_event(std::ostream &o, event const &e,
                                     std::size_t partialno, const size_t ndims)
{
  o << "(" << e.partial_type(partialno) << " ";
  if (ndims > 0)
    o << e.coord(partialno, 0);

  for (std::size_t dimno = 1; dimno < ndims; ++dimno)
    o << "," << e.coord(partialno, dimno);

  o << ")";
}

void event::nice_print(std::ostream &o) const
{
  o << "t=" << this->time() << ":";
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
  for (std::size_t partialno = 0; partialno < npartials; ++partialno)
  {
    o << " " << static_cast<EVENT_KIND_num>(this->partial_type(partialno));
    for (std::size_t dimno = 0; dimno < ndims; ++dimno)
      o << " " << this->coord(partialno, dimno) << " ";
  }
}
END_NAMESPACE_CORE

std::ostream &operator<<(std::ostream &o, CORE_NAMESPACE::event const &e)
{

  e.nice_print(o);
  return o;
}
