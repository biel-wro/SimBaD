#ifndef STREAM_PRINTER_HPP
#define STREAM_PRINTER_HPP

#include "interface/interface_fwd.hpp"

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

  void set_ostream(std::ostream *ostream);
  void set_ostream(std::ostream &ostream);
  void set_stream(std::string const &stream_name);
  std::ostream &ostream();

  virtual void write_dataframe(dataframe const &conf) final;
  virtual void write_header(dataframe const &conf) final;
  virtual void write_data(dataframe const &conf);

  virtual void write_header(attribute_description const &desc) = 0;
  virtual void write_entry(attribute_list const &entry) = 0;
  virtual void write_footer() = 0;

protected:
  using stream_deleter = std::function<void(std::ostream *)>;
  using stream_ptr = std::unique_ptr<std::ostream, stream_deleter>;
  static stream_ptr stream_from_name(std::string const &stream_name);

  explicit stream_printer(stream_ptr);

private:
  stream_ptr m_ostream_ptr;
};

END_NAMESPACE_CORE
#endif // STREAM_PRINTER_HPP
