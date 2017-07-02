#ifndef STREAM_PRINTER_HPP
#define STREAM_PRINTER_HPP
#include "core_fwd.hpp"
#include <iosfwd>
BEGIN_NAMESPACE_CORE

class stream_printer
{
public:
  stream_printer(std::ostream *ostream);
  stream_printer(std::ostream &ostream);
  void set_ostream(std::ostream &ostream);
  std::ostream &ostream();

  virtual void write_dataframe(dataframe const &conf) final;
  virtual void write_header(dataframe const &conf) final;
  virtual void write_data(dataframe const &conf);

  virtual void write_header(attribute_description const &desc) = 0;
  virtual void write_entry(attribute_list const &entry) = 0;
  virtual void write_footer() = 0;

private:
  std::ostream *m_ostream_ptr;
};

END_NAMESPACE_CORE
#endif // STREAM_PRINTER_HPP
