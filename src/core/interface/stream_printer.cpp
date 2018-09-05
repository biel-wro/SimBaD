#include "stream_printer.hpp"

#include "interface/dataframe.hpp"
#include "utils/stream_from_name.hpp"

#include <assert.h>
#include <fstream>
#include <iostream>

BEGIN_NAMESPACE_CORE



stream_printer::stream_printer() : m_ostream_ptr(nullptr) {}
stream_printer::stream_printer(std::ostream *ptr)
    : m_ostream_ptr(ostream_from_unmanaged(ptr))
{
}
stream_printer::stream_printer(std::ostream &ref)
    : m_ostream_ptr(ostream_from_unmanaged(&ref))
{
}

stream_printer::stream_printer(std::string const &stream_name)
    : m_ostream_ptr(ostream_from_name(stream_name))
{
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
  write_header(conf.description());
}

void stream_printer::write_data(dataframe const &data)
{
  data.visit_records(
      [&](attribute_list const &entry) { this->write_entry(entry); });
}

stream_printer::stream_printer(ostream_ptr ptr)
    : m_ostream_ptr(std::move(ptr))
{
}


/*
void stream_printer::read_configuration(const dataframe &conf)
{
  write_header(conf);
  write_data(conf);
  write_footer(conf);
}*/
END_NAMESPACE_CORE
