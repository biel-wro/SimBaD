#include "stream_printer.hpp"

#include "interface/dataframe.hpp"

#include <assert.h>
#include <fstream>
#include <iostream>

BEGIN_NAMESPACE_CORE

namespace
{
struct nop
{
  template <class T> void operator()(T const &) const noexcept {}
};
}

stream_printer::stream_printer() : m_ostream_ptr(nullptr) {}
stream_printer::stream_printer(std::ostream *ostream)
    : m_ostream_ptr(ostream, nop())
{
}
stream_printer::stream_printer(std::ostream &ostream)
    : m_ostream_ptr(&ostream, nop())
{
}

stream_printer::stream_printer(std::string const &stream_name)
    : m_ostream_ptr(stream_from_name(stream_name))
{
}

void stream_printer::set_ostream(std::ostream *ostream)
{
  m_ostream_ptr = stream_ptr(ostream, nop());
}

void stream_printer::set_ostream(std::ostream &ostream)
{
  m_ostream_ptr = stream_ptr(&ostream, nop());
}

void stream_printer::set_stream(std::string const &stream_name)
{
  m_ostream_ptr = stream_from_name(stream_name);
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

stream_printer::stream_printer(simbad::core::stream_printer::stream_ptr ptr)
    : m_ostream_ptr(std::move(ptr))
{
}

stream_printer::stream_ptr
stream_printer::stream_from_name(std::string const &stream_name)
{
  if("STDIN" == stream_name)
    return stream_ptr(&std::cout, nop());

  std::ofstream *ostream_ptr = new std::ofstream(stream_name);

  return stream_ptr(ostream_ptr, [](std::ostream *ptr) {
    delete dynamic_cast<std::ofstream *>(ptr);
  });
}

/*
void stream_printer::read_configuration(const dataframe &conf)
{
  write_header(conf);
  write_data(conf);
  write_footer(conf);
}*/
END_NAMESPACE_CORE
