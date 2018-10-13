#ifndef STREAM_PRINTER_HPP
#define STREAM_PRINTER_HPP

#include "interface/interface_fwd.hpp"
#include "utils/stream_from_name.hpp"

#include <boost/optional/optional_fwd.hpp>

#include <functional>
#include <iosfwd>
#include <memory>

BEGIN_NAMESPACE_CORE
class stream_printer
{
public:
  stream_printer();
  explicit stream_printer(std::ostream *);
  explicit stream_printer(std::ostream &ostream);
  explicit stream_printer(std::string const &stream_name);
  virtual ~stream_printer() = default;

  std::ostream &ostream();

  virtual void write_dataframe(dataframe const &conf) final;
  virtual void write_header(dataframe const &conf) final;

  virtual void write_data(dataframe const &conf);

  virtual void write_header(attribute_description const &desc) = 0;
  virtual void write_entry(attribute_list const &entry) = 0;
  virtual void write_footer() = 0;

protected:
  explicit stream_printer(ostream_ptr);

private:
  ostream_ptr m_ostream_ptr;
};

END_NAMESPACE_CORE
#endif // STREAM_PRINTER_HPP
