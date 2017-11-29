#include "stream_printer.hpp"
#include <assert.h>

#include "interface/dataframe.hpp"

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

void stream_printer::write_dataframe(const dataframe &conf)
{
  write_header(conf);
  write_data(conf);
  write_footer();
}

void stream_printer::write_header(dataframe const &conf)
{
  write_header(conf.descriptor());
}

void stream_printer::write_data(dataframe const &data)
{
  data.visit_records(
      [&](attribute_list const &entry) { this->write_entry(entry); });
}

/*
void stream_printer::read_configuration(const dataframe &conf)
{
  write_header(conf);
  write_data(conf);
  write_footer(conf);
}*/
END_NAMESPACE_CORE
