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

void stream_printer::read_configuration(const configuration_view &conf)
{
  write_header(conf);
  write_data(conf);
  write_footer(conf);
}
END_NAMESPACE_CORE
