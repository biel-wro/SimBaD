#ifndef SIMBAD_CORE_STREAM_READER_HPP
#define SIMBAD_CORE_STREAM_READER_HPP

#include "interface/interface_fwd.hpp"
#include "utils/stream_from_name.hpp"

#include <functional>
#include <iosfwd>

BEGIN_NAMESPACE_CORE
class stream_reader
{
public:
  using entry_visitor = std::function<void(attribute_list const &)>;

  explicit stream_reader(std::istream *ptr);
  explicit stream_reader(std::istream &ref);
  explicit stream_reader(std::string const &stream_name);
  virtual ~stream_reader();

  void set_istream(std::istream &istream);
  std::istream &istream() const;

  virtual attribute_description read_header() = 0;

  virtual bool visit_entries(entry_visitor v, std::size_t max_reads = 0) = 0;

private:
  istream_ptr m_istream_ptr;
};
END_NAMESPACE_CORE

#endif // SIMBAD_STREAM_READER_HPP
