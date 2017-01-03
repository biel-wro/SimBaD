#include "stream_printer.hpp"
#include <assert.h>
BEGIN_NAMESPACE_CORE
stream_printer::stream_printer(std::ostream *ostream) : m_ostream_ptr(ostream)
{
}
stream_printer::stream_printer(std::ostream &ostream) : m_ostream_ptr(&ostream)
{
}

void stream_printer::set_ostream(std::ostream &ostream)
{
  m_ostream_ptr = &ostream;
}

std::ostream &stream_printer::ostream()
{
  assert(nullptr != m_ostream_ptr);
  return *m_ostream_ptr;
}
END_NAMESPACE_CORE
